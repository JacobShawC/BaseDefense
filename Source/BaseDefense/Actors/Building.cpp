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
}


void ABuilding::Tick(float DeltaSeconds)
{
	if (Constructor && !Constructor->IsActorBeingDestroyed())
	{
		FVector PlayerLocation = Constructor->GetActorLocation();
		FVector BuildingLocation = GetActorLocation();
		float Distance = FVector::Dist(PlayerLocation, BuildingLocation);
		if (Distance > GameInstance->DefaultPlayerData.BuildRangeHorizontal)
		{
			CancelConstruction();
		}
		CurrentConstructionTime += DeltaSeconds;
		if (FloatingInfo->IsValidLowLevelFast())
		{
			FloatingInfo->SetConstruction(CurrentConstructionTime);
		}
		if (CurrentConstructionTime > BuffedBuildingData.ConstructionTime)
		{
			SetUpBuilding(BuffedBuildingData.Building);
			if (FloatingInfo->IsValidLowLevelFast())
			{
				FloatingInfo->SetConstructionVisibility(false);
			}
			SetActorTickEnabled(false);
		}
	}
	else
	{
		CancelConstruction();
	}
}
void ABuilding::Construct(EBuilding ABuilding, APlayerChar* AConstructor)
{
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	Constructor = AConstructor;

	Building = ABuilding;
	if (GameInstance)
	{
		BaseBuildingData = *GameInstance->Buildings.Find(ABuilding);
		ApplyBuffs();

		SetUpBuilding(EBuilding::Construction);
		


		FloatingInfo->SetMaxConstruction(BuffedBuildingData.ConstructionTime);
		MaxConstructionTime = BuffedBuildingData.ConstructionTime;

		SetActorTickEnabled(true);
	}
	
}

void ABuilding::CancelConstruction()
{
	SetActorTickEnabled(false);
	Destroy();
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
	DOREPLIFETIME(ABuilding, MaxConstructionTime);
	DOREPLIFETIME(ABuilding, CurrentConstructionTime);
	DOREPLIFETIME(ABuilding, FloatingHeight);


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

void ABuilding::OnRep_SetConstruction()
{
	if (FloatingInfo == nullptr && FloatingWidget->IsValidLowLevelFast())
	{
		FloatingInfo = Cast<UFloatingBuildingInfo>(FloatingWidget->GetUserWidgetObject());
	}

	if (FloatingInfo != nullptr)
	{
		FloatingInfo->SetMaxConstruction(MaxConstructionTime);

		FloatingInfo->SetConstruction(CurrentConstructionTime);

	}
}

void ABuilding::OnRep_SetMaxConstruction()
{
	if (FloatingInfo == nullptr && FloatingWidget->IsValidLowLevelFast())
	{
		FloatingInfo = Cast<UFloatingBuildingInfo>(FloatingWidget->GetUserWidgetObject());
	}

	if (FloatingInfo != nullptr)
	{
		FloatingInfo->SetMaxConstruction(MaxConstructionTime);
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
	BuffedBuildingData = BaseBuildingData.ReturnWithBuffs(Buffs);
}
