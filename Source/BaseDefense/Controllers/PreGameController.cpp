// Fill out your copyright notice in the Description page of Project Settings.


#include "PreGameController.h"
#include "BDGameInstance.h"
#include "PreGame.h"
APreGameController::APreGameController(const FObjectInitializer& ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = false;

}

void APreGameController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetInputMode(FInputModeUIOnly());
}

void APreGameController::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());

	if (IsLocalPlayerController())
	{
		if (IsLocalPlayerController())
		{
			if (GameInstance)
			{
				TSubclassOf<UUserWidget>* GUIClass = (GameInstance->Widgets).Find("PreGame");
				PreGame = Cast<UPreGame>(CreateWidget<UUserWidget>(this, GUIClass->Get()));

				if (PreGame)
				{
					(PreGame)->AddToViewport();

					PreGame->Setup();
				}
			}
		}
	}
}
