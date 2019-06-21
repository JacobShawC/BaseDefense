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