// Fill out your copyright notice in the Description page of Project Settings.


#include "BDPlayerController.h"
#include <GameFramework/Pawn.h>
#include "PlayerChar.h"
#include <GameFramework/Actor.h>
#include <TimerManager.h>
#include <GameFramework/PlayerInput.h>
#include <UserWidget.h>
#include <UObjectIterator.h>
#include "GUI.h"
#include "BDGameInstance.h"
#include "BuildingGhost.h"
#include "Hotbar.h"
#include "BDGameState.h"
#include <Kismet/KismetMathLibrary.h>
ABDPlayerController::ABDPlayerController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = false;
	PrimaryActorTick.bCanEverTick = true;

}

void ABDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetInputMode(FInputModeGameAndUI());
}

void ABDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		TSubclassOf<UUserWidget>* GUIClass = (Cast<UBDGameInstance>(GetGameInstance())->Widgets).Find("GUI");
		GUIWidget = Cast<UGUI>(CreateWidget<UUserWidget>(this, GUIClass->Get()));

		if (GUIWidget)
		{
			GUIWidget->AddToViewport();
		}
	}
}

void ABDPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	MakeGhost();

	UpdateCommands();


}
void ABDPlayerController::MakeGhost()
{
	//if (!GetPawn()) return;
	////if you're not selecting a building on the hotbar then dont make the ghost
	//if (SelectedBuilding == EBuilding::None)
	//{
	//	if (BuildingGhost != nullptr)
	//	{
	//		BuildingGhost->Destroy();
	//		BuildingGhost = nullptr;
	//	}
	//	return;
	//}

	////Check if you are allowed to build here

	//ABDGameState* GameState = GetWorld()->GetGameState<ABDGameState>();
	//if (GameState == nullptr) return;

	//if (GameState->LevelGenerationActor == nullptr) return;

	//TArray<AActor*> ActorsToIgnore;
	//ActorsToIgnore.Add(GetPawn());

	//FHitResult HitResult = DoTraceMouse(ActorsToIgnore, ECC_Camera);
	//FRotator Rotat = UKismetMathLibrary::Conv_VectorToRotator(HitResult.ImpactNormal);
	//FString ActorString = "";
	//if (HitResult.GetActor())
	//{
	//	ActorString = HitResult.GetActor()->GetName();
	//}

	///*UE_LOG(LogTemp, Warning, TEXT("Rotator: %s, %s"),
	//	*Rotat.ToString(), *ActorString);*/

	//	//if the surface is uneven where your mouse is then dont make the ghost.
	//if (!(Rotat.Pitch <= 90 && Rotat.Pitch >= 50))
	//{
	//	if (BuildingGhost)
	//	{
	//		BuildingGhost->Destroy();
	//		BuildingGhost = nullptr;
	//	}
	//	return;
	//}

	//FVector RoundedMouseVector = FVector(FMath::RoundHalfToZero(HitResult.Location.X / 100) * 100, FMath::RoundHalfToZero(HitResult.Location.Y / 100) * 100, HitResult.Location.Z);
	//FVector PawnLocation = GetPawn()->GetActorLocation();

	//FHitResult RoundedHit = DoSingleTrace(FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z + 500), FVector(RoundedMouseVector.X, RoundedMouseVector.Y, PawnLocation.Z - 500), ActorsToIgnore, ECC_Camera);

	//float MouseVerticalDistance = FGenericPlatformMath::Abs(RoundedHit.Location.Z - HitResult.Location.Z);

	////if the mouse is far from the rounded location then dont make a ghost.
	//if (MouseVerticalDistance > 100)
	//{
	//	if (BuildingGhost)
	//	{
	//		BuildingGhost->Destroy();
	//		BuildingGhost = nullptr;
	//	}
	//	return;
	//}

	//FBuildingLocationInfo RoundedInfo = GetLocationInfo(RoundedHit);

	//if (!RoundedInfo.ClearFromBuildings)
	//{
	//	if (BuildingGhost)
	//	{
	//		BuildingGhost->Destroy();
	//		BuildingGhost = nullptr;
	//	}
	//	return;
	//}

	//if (BuildingGhost == nullptr)
	//{
	//	BuildingGhost = GetWorld()->SpawnActor<ABuildingGhost>(RoundedHit.Location, FRotator(0.0f));
	//	BuildingGhost->Initialise(GUIWidget->HotBar->GetSelectedBuilding(), RoundedInfo.Reachable, RoundedInfo.Buildable, RoundedInfo.Mineable);
	//}
	//else
	//{
	//	if (RoundedHit.Location != BuildingGhost->GetActorLocation())
	//	{
	//		//BuildingGhost->SetActorLocation(RoundedHit.Location);
	//		FHitResult Hit;

	//		BuildingGhost->SetActorLocation(RoundedHit.Location, false, &Hit, ETeleportType::TeleportPhysics);
	//	}
	//	BuildingGhost->SetBuilding(SelectedBuilding);
	//	BuildingGhost->SetBuildable(RoundedInfo.Buildable);
	//	BuildingGhost->SetReachable(RoundedInfo.Reachable);
	//	BuildingGhost->SetMineable(RoundedInfo.Mineable);

	//}

}
void ABDPlayerController::UpdateCommands()
{
	/*if (GUIWidget != nullptr && GUIWidget->IsValidLowLevel())
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
	}*/
}

void ABDPlayerController::ServerUpdatePositions_Implementation(FVector APawnPosition, FVector AMousePosition)
{
	Cast<APlayerChar>(GetPawn())->PawnLocation = APawnPosition;
	GetPawn()->SetActorLocation(APawnPosition);
}

bool ABDPlayerController::ServerUpdatePositions_Validate(FVector APawnPosition, FVector AMousePosition)
{
	return true;
}

void ABDPlayerController::ClearHUDWidgets_Implementation()
{
	/* Object Iterator for All User Widgets! */
	for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
	{
		UUserWidget* LiveWidget = *Itr;

		/* If the Widget has no World, Ignore it (It's probably in the Content Browser!) */
		if (!LiveWidget->GetWorld())
		{
			continue;
		}
		else
		{
			LiveWidget->RemoveFromParent();
		}
	}
}

bool ABDPlayerController::ClearHUDWidgets_Validate()
{
	return true;
}


void ABDPlayerController::GetKeysForAction(FName AnActionName, TArray<FInputActionKeyMapping>& ABindings)
{
	ABindings = PlayerInput->GetKeysForAction(AnActionName);
}

void ABDPlayerController::SetSelectedBuilding(EBuilding ABuilding)
{
	SelectedBuilding = ABuilding;


}

void ABDPlayerController::CancelAction()
{
	if (SelectedBuilding != EBuilding::None)
	{
		if (GUIWidget && GUIWidget->HotBar)
		{
			GUIWidget->HotBar->Select(-1);
		}
	}
}
