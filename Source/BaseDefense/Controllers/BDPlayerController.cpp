// Fill out your copyright notice in the Description page of Project Settings.

#include "BDPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/Interaction.h"
#include "Main/BDGameInstance.h"
#include "UI/GUI.h"
#include "UI/Hotbar.h"
#include "Engine/World.h"
#include "BDPlayerState.h"
#include "Public/DrawDebugHelpers.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/KismetMathLibrary.h"
#include "BuildingGhost.h"
#include "Building.h"
#include "PlayerChar.h"
#include "ConstructAction.h"
#include "BDPlayerState.h"
#include "UpgradeAction.h"
#include "GameFramework/PlayerInput.h"
#define COLLISION_BUILDABLE		ECC_GameTraceChannel1
#define COLLISION_BUILDING		ECC_GameTraceChannel2

DEFINE_LOG_CATEGORY(BDPlayerController);
//
//ABDPlayerController::ABDPlayerController()
//{
//	Super::APlayerController();
//	bShowMouseCursor = true;
//	bEnableMouseOverEvents = true;
//	bEnableTouchEvents = false;
//}

ABDPlayerController::ABDPlayerController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = false;
}		

void ABDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


	InputComponent->BindAxis("MoveForward", this, &ABDPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABDPlayerController::MoveRight);

	InputComponent->BindAction("Select", IE_Pressed, this, &ABDPlayerController::SelectPressed);
	InputComponent->BindAction("Select", IE_Released, this, &ABDPlayerController::SelectReleased);
	InputComponent->BindAction("SelectAlt", IE_Pressed, this, &ABDPlayerController::SelectAltPressed);
	InputComponent->BindAction("SelectAlt", IE_Released, this, &ABDPlayerController::SelectAltReleased);
	InputComponent->BindAction("Use", IE_Pressed, this, &ABDPlayerController::UsePressed);
	InputComponent->BindAction("Use", IE_Released, this, &ABDPlayerController::UseReleased);
	InputComponent->BindAction("UseAlt", IE_Pressed, this, &ABDPlayerController::UseAltPressed);
	InputComponent->BindAction("UseAlt", IE_Released, this, &ABDPlayerController::UseAltReleased);
	InputComponent->BindAction("Repair", IE_Pressed, this, &ABDPlayerController::RepairPressed);
	InputComponent->BindAction("Repair", IE_Released, this, &ABDPlayerController::RepairReleased);

	InputComponent->BindAction("Hotbar1", IE_Pressed, this, &ABDPlayerController::SelectHotbar<0>);
	InputComponent->BindAction("Hotbar2", IE_Pressed, this, &ABDPlayerController::SelectHotbar<1>);
	InputComponent->BindAction("Hotbar3", IE_Pressed, this, &ABDPlayerController::SelectHotbar<2>);
	InputComponent->BindAction("Hotbar4", IE_Pressed, this, &ABDPlayerController::SelectHotbar<3>);
	InputComponent->BindAction("Hotbar5", IE_Pressed, this, &ABDPlayerController::SelectHotbar<4>);
	InputComponent->BindAction("Hotbar6", IE_Pressed, this, &ABDPlayerController::SelectHotbar<5>);
	InputComponent->BindAction("Hotbar7", IE_Pressed, this, &ABDPlayerController::SelectHotbar<6>);
	InputComponent->BindAction("Hotbar8", IE_Pressed, this, &ABDPlayerController::SelectHotbar<7>);
	InputComponent->BindAction("Hotbar9", IE_Pressed, this, &ABDPlayerController::SelectHotbar<8>);
	InputComponent->BindAction("Hotbar10", IE_Pressed, this, &ABDPlayerController::SelectHotbar<9>);

	InputComponent->BindAction("Scroll Up", IE_Pressed, this, &ABDPlayerController::ScrollHotbar<false>);
	InputComponent->BindAction("Scroll Down", IE_Pressed, this, &ABDPlayerController::ScrollHotbar<true>);
	//SetInputMode(FInputModeGameAndUI());
}

void ABDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());

	if (IsLocalPlayerController())
	{
		if (IsLocalPlayerController())
		{
			if (GameInstance)
			{
				TSubclassOf<UUserWidget>* GUIClass = (GameInstance->Widgets).Find("GUI");
				GUIWidget = Cast<UGUI>(CreateWidget<UUserWidget>(this, GUIClass->Get()));

				if (GUIWidget)
				{
					(GUIWidget)->AddToViewport();
					UGUI* GUITemp = Cast<UGUI>(GUIWidget);
					ABDPlayerState* State = Cast<ABDPlayerState>(GetPlayerState<ABDPlayerState>());
					if (State)
					{
						GUITemp->SetMoneyText(State->Money);
					}
				}

			}
		}
	}
}

void ABDPlayerController::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		GetPawn()->AddMovementInput(Direction, Value);
	}
}

void ABDPlayerController::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// find out which way is right
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		GetPawn()->AddMovementInput(Direction, Value);
	}
}



void ABDPlayerController::ServerUpgradeBuilding_Implementation(class ABuilding* ABuilding)
{
	APlayerChar* PlayerChar = Cast<APlayerChar>(GetPawn());

	if (PlayerChar->CurrentAction == nullptr)
	{
		PlayerChar->UpgradeAction->UpgradeBuilding(ABuilding);
	}
}

bool ABDPlayerController::ServerUpgradeBuilding_Validate(class ABuilding* ABuilding)
{
	return true;
}

void ABDPlayerController::ServerCancelAction_Implementation()
{
	APlayerChar* PlayerChar = nullptr;
	PlayerChar = Cast<APlayerChar>(GetPawn());
	if (PlayerChar != nullptr && PlayerChar->CurrentAction != nullptr)
	{
		PlayerChar->CurrentAction->CancelAction();
	}
}

bool ABDPlayerController::ServerCancelAction_Validate()
{
	return true;
}

bool ABDPlayerController::ChangePlayerMoney(float AMoney)
{
	ABDPlayerState* TempState = nullptr;
	APlayerChar* PlayerChar = Cast<APlayerChar>(GetPawn());

	TempState = Cast<ABDPlayerState>(PlayerState);

	if (TempState != nullptr)
	{
		if (TempState->Money + AMoney > 0)
		{
			TempState->Money += AMoney;
			TempState->OnRep_Money();
			return true;
		}
	}
	return false;
}


void ABDPlayerController::ServerConstructBuilding_Implementation(EBuilding ABuildingEnum, FVector APosition)
{
	FBuildingData Data;
	Data = *GameInstance->Buildings.Find(ABuildingEnum);

	ABDPlayerState* TempState = nullptr;
	TempState = Cast<ABDPlayerState>(PlayerState);
	APlayerChar* PlayerChar = Cast<APlayerChar>(GetPawn());

	if (TempState!= nullptr  && PlayerChar->CurrentAction == nullptr)
	{
		FVector RoundedMouseVector = FVector(FMath::RoundHalfToZero(APosition.X / 100) * 100, FMath::RoundHalfToZero(APosition.Y / 100) * 100, APosition.Z);

		TArray<AActor*> PlayerPawn;
		PlayerPawn.Add(Cast<AActor>(GetPawn()));
		FVector PawnLocation = GetPawn()->GetActorLocation();

		FHitResult RoundedHit = DoSingleTrace(FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z + 500), FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z - 500), PlayerPawn, ECC_Camera);


		FBuildingLocationInfo TraceInfo = GetLocationInfo(RoundedHit);
		if (TraceInfo.Buildable && TraceInfo.ClearFromBuildings && TraceInfo.EvenSurface && TraceInfo.Reachable)
		{	
			if (PlayerChar->CurrentAction == nullptr)
			{
				PlayerChar->ConstructAction->ConstructBuilding(Data, APosition);
			}
		}
	}
}

bool ABDPlayerController::ServerConstructBuilding_Validate(EBuilding ABuildingEnum, FVector APosition)
{
	return true;
}

void ABDPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	MakeGhost();

	UpdateCommands();


}

FBuildingLocationInfo ABDPlayerController::GetLocationInfo(FHitResult ATrace)
{
	FBuildingLocationInfo LocationInfo;
	TArray<AActor*> PlayerPawn;

	PlayerPawn.Add(Cast<AActor>(GetPawn()));

	FVector PawnLocation = GetPawn()->GetActorLocation();
	FRotator TraceRotat = UKismetMathLibrary::Conv_VectorToRotator(ATrace.ImpactNormal);

	FPlayerData PlayerData = GetPlayerState<ABDPlayerState>()->PlayerData;

	//if the surface is uneven
	if (!(TraceRotat.Pitch <= 90 && TraceRotat.Pitch >= 50))
	{
		LocationInfo.EvenSurface = false;
	}
	else
	{
		LocationInfo.EvenSurface = true;
	}


	FHitResult BuildableHit = DoSingleTrace(ATrace.TraceStart, ATrace.Location, PlayerPawn, COLLISION_BUILDABLE);
	FHitResult BuildingHit = DoSingleTrace(ATrace.TraceStart, ATrace.Location, PlayerPawn, COLLISION_BUILDING);
	if (BuildableHit.GetActor())
	{
		LocationInfo.Buildable = true;
	}
	else
	{
		LocationInfo.Buildable = false;
	}

	if (BuildingHit.GetActor())
	{
		LocationInfo.ClearFromBuildings = false;
	}
	else
	{
		LocationInfo.ClearFromBuildings = true;
	}

	float HorizontalDistance = FVector::Dist2D(GetPawn()->GetActorLocation(), ATrace.Location);
	float VerticalDistance = FGenericPlatformMath::Abs(ATrace.Location.Z - PawnLocation.Z);
	if (VerticalDistance < PlayerData.BuildRangeVertical && HorizontalDistance < PlayerData.BuildRangeHorizontal)
	{
		LocationInfo.Reachable = true;
	}
	else
	{
		LocationInfo.Reachable = false;
	}

	return LocationInfo;
}

void ABDPlayerController::MakeGhost()
{
	if (!GetPawn()) return;
	//if you're currectly selecting something then dont make a ghost;
 	if (CurrentlySelected)
	{
		UObject* SelectedObject = Cast<UObject>(CurrentlySelected.GetObject());
		if (SelectedObject && SelectedObject->IsValidLowLevel() && IsSelectedValid())
		{
			if (BuildingGhost)
			{
				BuildingGhost->Destroy();
				BuildingGhost = nullptr;
			}
			return;
		}
	}

	if (!GUIWidget) return;

	//if you're not selecting a building on the hotbar then dont make the ghost
	EBuilding SelectedBuilding = GUIWidget->HotBar->GetSelectedBuilding();
	if (SelectedBuilding == EBuilding::None)
	{
		if (BuildingGhost)
		{
			BuildingGhost->Destroy();
			BuildingGhost = nullptr;
		}
		return;
	}

	//Check if you are allowed to build here
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetPawn());

	FHitResult HitResult = DoTraceMouse(ActorsToIgnore, ECC_Camera);
	FRotator Rotat = UKismetMathLibrary::Conv_VectorToRotator(HitResult.ImpactNormal);
	FString ActorString = "";
	if (HitResult.GetActor())
	{
		ActorString = HitResult.GetActor()->GetName();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Rotator: %s, %s"),
		*Rotat.ToString(), *ActorString);

	//if the surface is uneven where your mouse is then dont make the ghost.
	if (!(Rotat.Pitch <= 90 && Rotat.Pitch >= 50))
	{
		if (BuildingGhost)
		{
			BuildingGhost->Destroy();
			BuildingGhost = nullptr;
		}
		return;
	}

	FVector RoundedMouseVector = FVector(FMath::RoundHalfToZero(HitResult.Location.X / 100) * 100, FMath::RoundHalfToZero(HitResult.Location.Y / 100) * 100, HitResult.Location.Z);
	FVector PawnLocation = GetPawn()->GetActorLocation();

	FHitResult RoundedHit = DoSingleTrace(FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z + 500), FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z - 500), ActorsToIgnore, ECC_Camera);

	float MouseVerticalDistance = FGenericPlatformMath::Abs(RoundedHit.Location.Z - HitResult.Location.Z);

	//if the mouse is far from the rounded location then dont make a ghost.
	if (MouseVerticalDistance > 100)
	{
		if (BuildingGhost)
		{
			BuildingGhost->Destroy();
			BuildingGhost = nullptr;
		}
		return;
	}

	FBuildingLocationInfo RoundedInfo = GetLocationInfo(RoundedHit);

	if (!RoundedInfo.ClearFromBuildings)
	{
		if (BuildingGhost)
		{
			BuildingGhost->Destroy();
			BuildingGhost = nullptr;
		}
		return;
	}

	if (BuildingGhost == nullptr)
	{
		BuildingGhost = GetWorld()->SpawnActor<ABuildingGhost>(RoundedHit.Location, FRotator(0.0f));
		BuildingGhost->Initialise(GUIWidget->HotBar->GetSelectedBuilding(), RoundedInfo.Reachable, RoundedInfo.Buildable);
	}
	else
	{
		if (RoundedHit.Location != BuildingGhost->GetActorLocation())
		{
			//BuildingGhost->SetActorLocation(RoundedHit.Location);
			FHitResult Hit;

			BuildingGhost->SetActorLocation(RoundedHit.Location, false, &Hit, ETeleportType::TeleportPhysics);
		}
		BuildingGhost->SetBuilding(SelectedBuilding);
		BuildingGhost->SetBuildable(RoundedInfo.Buildable);
		BuildingGhost->SetReachable(RoundedInfo.Reachable);
	}
}

void ABDPlayerController::UpdateCommands()
{
	if (GUIWidget != nullptr && GUIWidget->IsValidLowLevel())
	{
		if (IsSelectedValid())
		{
			TArray<EGUICommand> Commands = CurrentlySelected->GetCommands();
			GUIWidget->SetCommandList(Commands);
		}
		else
		{
			GUIWidget->SetCommandList(TArray<EGUICommand>());
		}
	}
}

int AddCost(FBuildingData ABuildingData, EBuildingUpgrade AnUpgrade)
{
	int ReturnCost = 0;

	return ReturnCost;

}



void ABDPlayerController::ServerSelectBuildingUpgrade_Implementation(EBuilding ABuilding, EBuildingUpgrade AnUpgrade, bool AddOrRemove)
{
 	if (GetPawn()->GetPlayerState() != nullptr)
	{
		Cast<ABDPlayerState>(GetPawn()->GetPlayerState())->SelectBuildingUpgrade(ABuilding, AnUpgrade, AddOrRemove);
	}
}

bool ABDPlayerController::ServerSelectBuildingUpgrade_Validate(EBuilding ABuilding, EBuildingUpgrade AnUpgrade, bool AddOrRemove)
{
	return true;
}

void ABDPlayerController::GetKeysForAction(FName AnActionName, TArray<FInputActionKeyMapping>& Bindings)
{
	Bindings = PlayerInput->GetKeysForAction(AnActionName);
}

void ABDPlayerController::UpdateGhost(bool AReachable, bool ABuildable)
{
	
}



FHitResult ABDPlayerController::DoTraceMouse(TArray<AActor*> ActorsToIgnore, ECollisionChannel AChannel)
{
	FHitResult TempHitData(ForceInit);

	if (World)
	{
		FVector WorldLocation;
		FVector WorldDirection;
		DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		FVector End = WorldLocation + WorldDirection * 10000;
		TempHitData = DoSingleTrace(WorldLocation, End, ActorsToIgnore, AChannel);
		//DrawDebugLine(GetWorld(), WorldLocation, End, FColor::Red, true);

	}
	return TempHitData;
}

FHitResult ABDPlayerController::RoundedMouseTrace(ECollisionChannel AChannel)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetPawn());

	FHitResult HitResult = DoTraceMouse(ActorsToIgnore, ECC_Camera);
	FVector RoundedMouseVector = FVector(FMath::RoundHalfToZero(HitResult.Location.X / 100) * 100, FMath::RoundHalfToZero(HitResult.Location.Y / 100) * 100, HitResult.Location.Z);
	FVector PawnLocation = GetPawn()->GetActorLocation();

	//DrawDebugLine(GetWorld(), FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z + 500), FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z - 500), FColor::Red, true);
	return DoSingleTrace(FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z + 500), FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z - 500), ActorsToIgnore, ECC_Camera);
}

FHitResult ABDPlayerController::DoSingleTrace(FVector Start, FVector End, TArray<AActor*> ActorsToIgnore, ECollisionChannel AChannel)
{
	FHitResult TempHitData(ForceInit);
	if (World)
	{
		FCollisionQueryParams QTraceParams;
		QTraceParams.AddIgnoredActors(ActorsToIgnore);

		World->LineTraceSingleByChannel(
		TempHitData,
		Start,
		End,
		AChannel,
		QTraceParams,
		FCollisionResponseParams()
		);

		//DrawDebugLine(World, TempHitData.TraceStart, TempHitData.TraceEnd, FColor::Red, false, 0.1f, 0, 0.1f);

	}
	return TempHitData;
}





bool ABDPlayerController::IsSelectedValid()
{
	if (CurrentlySelected != nullptr)
	{
		UObject* SelectedObject = nullptr;
		SelectedObject = Cast<UObject>(CurrentlySelected.GetObject());

		if (SelectedObject != nullptr && !SelectedObject->IsPendingKill())
		{
			return true;
		}
	}
	return false;
}

void ABDPlayerController::SelectPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("SelectPressed()"));

	if (IsSelectedValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectPressed() Valid"));

		CurrentlySelected->SelectPressed();
	}
	else
	{
		if (BuildingGhost && BuildingGhost->Buildable && BuildingGhost->Reachable)
		{
			FVector GhostLocation = BuildingGhost->GetActorLocation();
			FRotator GhostRotation = BuildingGhost->GetActorRotation();
			
			ServerConstructBuilding(BuildingGhost->Building, GhostLocation);
			BuildingGhost->Destroy();
			BuildingGhost = nullptr;
		}
	}
}

void ABDPlayerController::SelectReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("SelectReleased()"));

	if (IsSelectedValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectReleased() Valid"));

		CurrentlySelected->SelectReleased();
	}
}

void ABDPlayerController::SelectAltPressed()
{
	APlayerChar* PlayerChar = nullptr;
	PlayerChar = Cast<APlayerChar>(GetPawn());
	if (PlayerChar != nullptr && PlayerChar->CurrentAction != nullptr)
	{
		ServerCancelAction();
	}
	else
	{
		if (IsSelectedValid())
		{
			CurrentlySelected->SelectAltPressed();
		}
	}	
}

void ABDPlayerController::SelectAltReleased()
{
	if (IsSelectedValid())
	{
		CurrentlySelected->SelectAltReleased();
	}
}

void ABDPlayerController::UsePressed()
{
	if (IsSelectedValid())
	{
		CurrentlySelected->UsePressed();
	}
}

void ABDPlayerController::UseReleased()
{
	if (IsSelectedValid())
	{
		CurrentlySelected->UseReleased();
	}
}

void ABDPlayerController::UseAltPressed()
{
	if (IsSelectedValid())
	{
		CurrentlySelected->UseAltPressed();
	}
}

void ABDPlayerController::UseAltReleased()
{
	if (IsSelectedValid())
	{
		CurrentlySelected->UseAltReleased();
	}
}

void ABDPlayerController::RepairPressed()
{
	if (IsSelectedValid())
	{
		CurrentlySelected->RepairPressed();
	}
}

void ABDPlayerController::RepairReleased()
{
	if (IsSelectedValid())
	{
		CurrentlySelected->RepairReleased();
	}
}

void ABDPlayerController::SelectHotbar(int ASlot)
{
	if (GUIWidget && GUIWidget->HotBar)
	{
		GUIWidget->HotBar->Select(ASlot);
	}
}
bool ABDPlayerController::GetActionHeld(FName AText)
{
	TArray < FInputActionKeyMapping > KeyMappings = PlayerInput->GetKeysForAction(AText);
	for (auto Mapping : KeyMappings)
	{
		if (IsInputKeyDown(Mapping.Key))
		{
			return true;
		}
	}

	return false;
}

void ABDPlayerController::SetCurrentlySelected(TScriptInterface<IInteraction> ACurrentlySelected)
{
	CurrentlySelected = ACurrentlySelected;

	if (GetActionHeld("Repair"))
	{
		CurrentlySelected->RepairPressed();
	}


}

//void ABDPlayerController::TryBuildBuildingFromGhost()
//{
//	if (BuildingGhost && BuildingGhost->Buildable && BuildingGhost->Reachable)
//	{
//		FVector GhostLocation = BuildingGhost->GetActorLocation();
//		FRotator GhostRotation = BuildingGhost->GetActorRotation();
//		ABuilding* Building = nullptr;
//		Building = GetWorld()->SpawnActor<ABuilding>(GhostLocation, GhostRotation);
//		if (Building)
//		{
//			Building->Initialise(BuildingGhost->Building);
//		}
//		BuildingGhost->Destroy();
//		BuildingGhost = nullptr;
//	}
//}

void ABDPlayerController::ScrollHotbar(bool UpOrDown)
{
	if (GUIWidget && GUIWidget->HotBar)
	{
		GUIWidget->HotBar->Scroll(UpOrDown);
	}
}
