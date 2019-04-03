// Fill out your copyright notice in the Description page of Project Settings.
#include "Building.h"
#include "BDGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "FloatingBuildingInfo.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/BoxSphereBounds.h"
#include "Math/Box.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "PlayerChar.h"
#include "BuildingAIAction.h"
#include "BuildingAttackAIAction.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "BDGameState.h"
#include "Public/TimerManager.h"
#include "DamageTextActor.h"
#include "RepairAction.h"
#include "UpgradeAction.h"
#include "BDPlayerState.h"
#include "BDPlayerController.h"
// Sets default values
ABuilding::ABuilding()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//MeshComponent->SetSimulatePhysics(false);
	//MeshComponent->SetCollisionProfileName("Building");
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetIsReplicated(true);
	//MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	//MeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	//MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComponent->SetCollisionProfileName("BuildingMesh");
	MeshComponent->OnBeginCursorOver.AddDynamic(this, &ABuilding::OnMouseEnter);
	MeshComponent->OnEndCursorOver.AddDynamic(this, &ABuilding::OnMouseLeave);
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCustomDepthStencilValue(1);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->SetIsReplicated(true);

	OverlapCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlapCapsule"));
	OverlapCapsule->SetCollisionProfileName("Building");
	OverlapCapsule->SetGenerateOverlapEvents(true);
	OverlapCapsule->SetCapsuleSize(50, 50, true);
	OverlapCapsule->bVisible = true;
	OverlapCapsule->SetupAttachment(RootComponent);
	OverlapCapsule->SetRelativeLocation(FVector(0, 0, 25));
	FloatingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingInfo"));
	static ConstructorHelpers::FClassFinder<UUserWidget> FloatingWidgetClass(TEXT("WidgetBlueprint'/Game/UI/FloatingBuildingInfoWidget.FloatingBuildingInfoWidget_C'"));
	FloatingWidget->SetWidgetClass(FloatingWidgetClass.Class);
	FloatingWidget->SetDrawAtDesiredSize(true);
	FloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingWidget->SetupAttachment(RootComponent);
	FloatingWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent->OnAttacked.AddUObject(this, &ABuilding::OnAttacked);
}

void ABuilding::GiveMoney(float AnAmount, bool AToOwningPlayer, bool APenalty)
{
	float UpdatedAmount = AnAmount;

	//if we need to apply a penalty
	if (APenalty)
	{
		UpdatedAmount = AnAmount * BuffedBuildingData.CostPenalty;
	}

	if (AToOwningPlayer && OwningCharacter != nullptr && OwningCharacter->IsValidLowLevelFast())
	{
		//give to owning player
		ABDPlayerState* OwningPlayerState = OwningCharacter->GetPlayerState<ABDPlayerState>();
		OwningPlayerState->ChangePlayerMoney(UpdatedAmount);
	}
	else
	{
		//divide up
		
		if (GameState == nullptr)
		{
			GameState = GetWorld() != NULL ? GetWorld()->GetGameState<ABDGameState>() : nullptr;
		}

		if (GameState != nullptr)
		{
			float DividedIncome = GameState->AddMoney(UpdatedAmount);
		}
	}
	MulticastSpawnDamageText(FString::SanitizeFloat(UpdatedAmount), FColor::Yellow);
}

void ABuilding::CancelInteraction()
{
	CurrentInteractionTime = 0;

	if (Interacting)
	{
		Interacting = false;

		if (CurrentInteraction.RefundOnFail)
		{
			GiveMoney(CurrentInteraction.Cost, true, false);
		}
		if (CurrentInteraction.DestroyOnFail)
		{
			Destroy();
			return;
		}
		else if (CurrentInteraction.UsesConstructionBuilding)
		{
			SetUpBuilding(BuffedBuildingData.Building);
		}
	}

	if (FloatingInfo->IsValidLowLevelFast())
	{

		OnRep_SetInteraction();
	}
}

void ABuilding::CompleteInteraction()
{
	CurrentInteractionTime = 0;

	if (Interacting)
	{
		Interacting = false;

		if (CurrentInteraction.UsesConstructionBuilding)
		{
			SetUpBuilding(BuffedBuildingData.Building);
		}

		if (FloatingInfo->IsValidLowLevelFast())
		{
			OnRep_SetInteraction();
		}

		if (CurrentInteraction.Type == EBuildingInteractionType::Construct && CurrentInteraction.Building != EBuilding::None)
		{
			Constructing = false;
			SetUpBuilding(CurrentInteraction.Building);
			UpdateUpgradable();
		}

		if (CurrentInteraction.Type == EBuildingInteractionType::Destroy)
		{
			Destroy();
		}

		if (CurrentInteraction.Type == EBuildingInteractionType::Sell)
		{
			GiveMoney(CurrentInteraction.Cost, true, true);

			Destroy();
		}

		if (CurrentInteraction.Type == EBuildingInteractionType::Upgrade)
		{
			switch (CurrentUpgrade)
			{
			case EBuildingUpgrade::None: CurrentUpgrade = EBuildingUpgrade::Level2; break;
			case EBuildingUpgrade::Level2: CurrentUpgrade = EBuildingUpgrade::Level3; break;
			case EBuildingUpgrade::Level3: CurrentUpgrade = EBuildingUpgrade::Level4; break;
			}
			UpdateUpgradable();
			ApplyBuffs();
		}
	}
}

TArray<EGUICommand> ABuilding::GetCommands()
{
	TArray<EGUICommand> ReturnArr;
	if (HealthComponent->Health < HealthComponent->MaxHealth)
	{
		ReturnArr.Add(EGUICommand::Repair);
	}

	if (Upgradable)
	{
		ReturnArr.Add(EGUICommand::Upgrade);
	}

	return ReturnArr;
}

void ABuilding::TickInteraction(float DeltaSeconds)
{
	if (Interacting)
	{
		CurrentInteractionTime += DeltaSeconds;
		if (FloatingInfo->IsValidLowLevelFast())
		{

			OnRep_SetInteraction();
		}

		if (CurrentInteraction.RequiresPlayer)
		{
			FVector PlayerLocation = CurrentInteraction.Interactor->GetActorLocation();
			FVector BuildingLocation = GetActorLocation();
			float Distance = FVector::Dist(PlayerLocation, BuildingLocation);
			if (Distance > CurrentInteraction.Range)
			{
				CancelInteraction();
				return;
			}
		}

		if (CurrentInteractionTime > BuffedBuildingData.ConstructionTime)
		{
			CompleteInteraction();
			return;
		}
	}
}


void ABuilding::Tick(float DeltaSeconds)
{
	if (Interacting)
	{
		TickInteraction(DeltaSeconds);
	}

	if (!Interacting)
	{
		SetActorTickEnabled(false);
	}
}

void ABuilding::Interact(FBuildingInteraction AnInteraction)
{
	if (!Interacting && AnInteraction.Interactor->IsValidLowLevel() && !AnInteraction.Interactor->IsPendingKill())
	{
		CurrentInteractionTime = 0.0f;
		CurrentInteraction = AnInteraction;
		Interacting = true;
		SetActorTickEnabled(true);
	}
}


void ABuilding::Construct(EBuilding ABuilding, APlayerChar* AConstructor)
{
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	OwningCharacter = AConstructor;
	Building = ABuilding;
	if (GameInstance)
	{
		BaseBuildingData = *GameInstance->Buildings.Find(ABuilding);

		ApplyBuffs();
		
		//TODO: get initial buffs here
		FBuildingBuffStruct Test1(EBuildingBuffType::ConstructionSpeed, EBuffOperator::Add, 4);
		FBuildingBuffStruct Test2 = Test1;
		Test2.Duration = 5.0f;

		//AddBuff(Test1);
		//AddBuff(Test2);
		ApplyBuffs();
		
		

		SetUpBuilding(EBuilding::Construction);

		MaxInteractionTime = BuffedBuildingData.ConstructionTime;

		Constructing = true;

		FBuildingInteraction ConstructInteraction;
		ConstructInteraction.Type = EBuildingInteractionType::Construct;
		ConstructInteraction.Interactor = AConstructor;
		ConstructInteraction.Cost = BuffedBuildingData.Cost;
		ConstructInteraction.Building = BuffedBuildingData.Building;
		ConstructInteraction.Duration = BuffedBuildingData.ConstructionTime;
		ConstructInteraction.DestroyOnFail = true;
		ConstructInteraction.RefundOnFail = true;
		ConstructInteraction.Range = AConstructor->PlayerData.BuildRangeHorizontal; 
		Interact(ConstructInteraction);
	}
	
}




void ABuilding::Upgrade(EBuildingUpgrade AnUpgrade, APlayerChar * AnUpgrader)
{
	UpdateUpgradable();


	if (!Interacting && Upgradable)
	{
		FBuildingInteraction Interaction;
		Interaction.Type = EBuildingInteractionType::Upgrade;
		Interaction.Interactor = AnUpgrader;
		Interaction.Building = BuffedBuildingData.Building;
		Interaction.Duration = BuffedBuildingData.Upgrades.Find(AnUpgrade)->UpgradeTime;
		Interaction.Cost = BuffedBuildingData.Upgrades.Find(AnUpgrade)->Cost;
		Interaction.DestroyOnFail = false;
		Interaction.RefundOnFail = true;
		Interaction.Range = AnUpgrader->PlayerData.BuildRangeHorizontal;
		Interact(Interaction);
	}
}



void ABuilding::SetUpBuilding(EBuilding ABuilding)
{
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	Building = ABuilding;
	FBuildingData TempBuildingData;
	if (GameInstance)
	{
		TempBuildingData = *GameInstance->Buildings.Find(ABuilding);
		if (FloatingWidget)
		{
			FloatingInfo = Cast<UFloatingBuildingInfo>(FloatingWidget->GetUserWidgetObject());
		}
		if (FloatingInfo)
		{
			FloatingInfo->SetName(TempBuildingData.Name);
			FloatingInfo->SetNameVisibility(false);
		}
		HealthComponent->Initialise(TempBuildingData.MaxHealth);
		UStaticMesh* Mesh = nullptr;
		Mesh = TempBuildingData.Mesh;
		if (Mesh)
		{
			if (MeshComponent)
			{
				MeshComponent->SetStaticMesh(Mesh);
				MeshComponent->SetRelativeScale3D(FVector(TempBuildingData.MeshScale));
				float MeshHeight = Mesh->GetBounds().BoxExtent.Z;
				MeshHeight = (MeshHeight * TempBuildingData.MeshScale) + 75;

				//FloatingWidget->SetRelativeLocation(FVector(0, 0, MeshHeight));
				FloatingHeight = MeshHeight;
				OnRep_SetFloatingHeight();
			}
		}
	}

	UBuildingAIAction* Action = nullptr;
	if (TempBuildingData.Attack.AttackType != EAttackType::None)
	{
		Action = NewObject<UBuildingAttackAIAction>();
		Actions.Add(Action);
		Action->Initialise(this);
		Action = nullptr;
	}

	if (TempBuildingData.Properties.Contains(EBuildingProperty::Income))
	{
		GenerateIncome();
	}

	if (Role == ROLE_Authority)
	{
		WhatDo();
	}

}

void ABuilding::OnMouseEnter(UPrimitiveComponent * TouchedComponent)
{
	if (FloatingInfo)
	{
		FloatingInfo->SetNameVisibility(true);
	}
	MeshComponent->SetRenderCustomDepth(true);

	if (Controller == nullptr)
	{
		Controller = Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController());
	}
	if (Controller != nullptr)
	{
		Controller->SetCurrentlySelected(this);
	}
}

void ABuilding::OnMouseLeave(UPrimitiveComponent * TouchedComponent)
{
	if (FloatingInfo)
	{
		FloatingInfo->SetNameVisibility(false);
	}
	MeshComponent->SetRenderCustomDepth(false);

	if (Character == nullptr)
	{
		Character = Cast<APlayerChar>(GetWorld()->GetFirstLocalPlayerFromController());
	}


	if (Controller == nullptr)
	{
		Controller = Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController());
	}
	if (Controller != nullptr)
	{
		if (Controller->CurrentlySelected == this)
		{
			Controller->CurrentlySelected = nullptr;
		}
	}

}

void ABuilding::OnAttacked(AActor* AttackingTarget, float ADamage)
{
	if (Interacting && CurrentInteraction.CancelOnDamage)
	{
		CancelInteraction();
	}
}

bool ABuilding::RepairPressed()
{
	if (Controller == nullptr)
	{
		Controller = Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController());
	}
	if (Controller != nullptr)
	{
		Character = Cast<APlayerChar>(Controller->GetPawn());
	}

	if (Character != nullptr && Character->RepairAction != nullptr)
	{
		return (Character->RepairAction->RepairBuilding(this));
	}
	return false;
}

bool ABuilding::UsePressed()
{
	if (Controller == nullptr)
	{
		Controller = Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController());
	}
	if (Controller != nullptr)
	{
		Character = Cast<APlayerChar>(Controller->GetPawn());
	}

	if (Character != nullptr && Character->UpgradeAction != nullptr)
	{
		ABDPlayerController* CharController = nullptr;
		CharController = Cast<ABDPlayerController>(Character->GetController());
		if (CharController != nullptr)
		{
			CharController->ServerUpgradeBuilding(this);
			return true;
		}
	}
	return false;
}

//bool ABuilding::RepairReleased()
//{
//	if (Character == nullptr)
//	{
//		Character = Cast<APlayerChar>(GetWorld()->GetFirstLocalPlayerFromController());
//	}
//
//	/*if (Character != nullptr && Character->RepairAction != nullptr && Character->RepairAction->Repairing == true && Character->RepairAction->Building == this)
//	{
//		Character->RepairAction->CancelRepair(ERepairCancelReason::Cancelled);
//		return true;
//	}*/
//	return false;
//}

void ABuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuilding, MaxInteractionTime);
	DOREPLIFETIME(ABuilding, CurrentInteractionTime);
	DOREPLIFETIME(ABuilding, FloatingHeight);
	DOREPLIFETIME(ABuilding, Upgradable);


}

void ABuilding::GenerateIncome()
{
	if (Role == ROLE_Authority)
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &ABuilding::GenerateIncome, BuffedBuildingData.Income.Cooldown, false);
		if (GameState == nullptr)
		{
			GameState = GetWorld() != NULL ? GetWorld()->GetGameState<ABDGameState>() : nullptr;
		}
		
		if (GameState != nullptr)
		{
			

			float DividedIncome = GameState->AddMoney(BuffedBuildingData.Income.IncomeAmount);
			MulticastSpawnDamageText(FString::SanitizeFloat(DividedIncome), FColor::Yellow);
		}

	}
	
}

void ABuilding::MulticastSpawnDamageText_Implementation(const FString& AText, FColor AColor)
{
	if (WasRecentlyRendered())
	{
		ADamageTextActor* DamageTextActor = nullptr;
		FVector OwnerLoc = GetActorLocation();
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		DamageTextActor = Cast<ADamageTextActor>(GetWorld()->SpawnActor<ADamageTextActor>(ADamageTextActor::StaticClass(), OwnerLoc, FRotator(0.0f), SpawnParams));
		if (DamageTextActor != nullptr)
		{
			DamageTextActor->Initialise(AText, AColor);
		}
	}
}


void ABuilding::WhatDo()
{
	if (IsActorBeingDestroyed() || CurrentAction != nullptr && CurrentAction->Executing && !CurrentAction->SafeToAbort)
	{
		return;
	}
	for (int i = 0; i < Actions.Num(); i++)
	{
		/*if (CurrentAction != nullptr && CurrentAction == Actions[i])
		{
			continue;
		}*/
		if (Actions[i]->ShouldActivate == true)
		{
			if (CurrentAction)
			{
				CurrentAction->Abort();
			}
			Actions[i]->Activate();
			CurrentAction = Actions[i];
			break;
		}
	}
}

void ABuilding::OnRep_SetInteraction()
{
	if (FloatingInfo == nullptr || !FloatingWidget->IsValidLowLevelFast())
	{
		FloatingInfo = Cast<UFloatingBuildingInfo>(FloatingWidget->GetUserWidgetObject());
	}

	if (FloatingInfo != nullptr && FloatingInfo->IsValidLowLevelFast())
	{
		if (CurrentInteractionTime == 0)
		{
			FloatingInfo->SetConstructionVisibility(false);
		}
		else
		{
			FloatingInfo->SetConstructionVisibility(true);
			FloatingInfo->SetMaxConstruction(MaxInteractionTime);

			FloatingInfo->SetConstruction(CurrentInteractionTime);
		}
	}
}

void ABuilding::OnRep_SetMaxInteraction()
{
	if (FloatingInfo == nullptr || !FloatingWidget->IsValidLowLevelFast())
	{
		FloatingInfo = Cast<UFloatingBuildingInfo>(FloatingWidget->GetUserWidgetObject());
	}

	if (FloatingInfo != nullptr && FloatingInfo->IsValidLowLevelFast())
	{
		if (CurrentInteractionTime == 0)
		{
			FloatingInfo->SetConstructionVisibility(false);
		}
		else
		{
			FloatingInfo->SetConstructionVisibility(true);
			FloatingInfo->SetMaxConstruction(MaxInteractionTime);

			FloatingInfo->SetConstruction(CurrentInteractionTime);
		}
	}
}

void ABuilding::OnRep_SetFloatingHeight()
{
	if (FloatingWidget != nullptr)
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().ClearTimer(FuzeTimerHandle);

		FloatingWidget->SetRelativeLocation(FVector(0, 0, FloatingHeight));
	}
	else
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &ABuilding::OnRep_SetFloatingHeight, 0.05f, false);
	}
}


void ABuilding::ApplyBuffs()
{
	TArray<EBuildingUpgrade> CurrentUpgradeArr;
	CurrentUpgradeArr.Add(CurrentUpgrade);
	BuffedBuildingData = BaseBuildingData.ReturnWithBuffs(Buffs, CurrentUpgradeArr);

	HealthComponent->ChangeMaxHealth(BuffedBuildingData.MaxHealth, true);
}

void ABuilding::AddBuff(FBuildingBuffStruct ABuff)
{
	ABuff.StartTime = GetWorld()->TimeSeconds;
	Buffs.Add(ABuff);
	FBuildingBuffStruct test;
	if (ABuff.Duration > 0.0f)
	{
		FTimerHandle UniqueHandle;
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ABuilding::RemoveExpiredBuffs);
		GetWorldTimerManager().SetTimer(UniqueHandle, RespawnDelegate, ABuff.Duration, false);
	}
}



void ABuilding::AddBuffs(TArray<FBuildingBuffStruct> ABuffs)
{
	for (FBuildingBuffStruct ABuff : ABuffs)
	{
		AddBuff(ABuff);
	}

	
}

void ABuilding::RemoveExpiredBuffs()
{
	float CurrentTime = GetWorld()->TimeSeconds;
	bool Removed = false;
	for (int i = Buffs.Num() - 1; i >= 0; i--)
	{

		if (Buffs[i].Duration > 0.0f && Buffs[i].StartTime + Buffs[i].Duration < CurrentTime)
		{
			Removed = true;

			Buffs.RemoveAt(i);
		}
	}

	if (Removed)
	{
		ApplyBuffs();
	}
}

bool ABuilding::UpdateUpgradable()
{
	bool ReturnBool = false;


	switch (CurrentUpgrade)
	{
		case EBuildingUpgrade::None: ReturnBool = BuffedBuildingData.Upgrades.Contains(EBuildingUpgrade::Level2); break;
		case EBuildingUpgrade::Level2: ReturnBool = BuffedBuildingData.Upgrades.Contains(EBuildingUpgrade::Level3); break;
		case EBuildingUpgrade::Level3: ReturnBool = BuffedBuildingData.Upgrades.Contains(EBuildingUpgrade::Level4); break;
	}

	Upgradable = ReturnBool;
	return ReturnBool;
}

