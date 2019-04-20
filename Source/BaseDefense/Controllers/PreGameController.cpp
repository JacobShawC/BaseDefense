// Fill out your copyright notice in the Description page of Project Settings.


#include "PreGameController.h"

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
				/*TSubclassOf<UUserWidget>* GUIClass = (GameInstance->Widgets).Find("MainMenu");
				MainMenuUserWidget = Cast<UMainMenu>(CreateWidget<UUserWidget>(this, GUIClass->Get()));

				if (MainMenuUserWidget)
				{
					(MainMenuUserWidget)->AddToViewport();

					MainMenuUserWidget->SetUp();
					GameInstance->RefreshFriendsList();
				}*/
			}
		}
	}
}
