// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuController.h"
#include "UI/MainMenu.h"
#include "BDGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "UObject/UObjectIterator.h"
#include "UObject/ConstructorHelpers.h"

AMenuController::AMenuController(const FObjectInitializer& ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = false;
}

void AMenuController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetInputMode(FInputModeUIOnly());
}

void AMenuController::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());

	if (IsLocalPlayerController())
	{
		if (IsLocalPlayerController())
		{
			if (GameInstance)
			{
				TSubclassOf<UUserWidget>* GUIClass = (GameInstance->Widgets).Find("MainMenu");
				MainMenuUserWidget = Cast<UMainMenu>(CreateWidget<UUserWidget>(this, GUIClass->Get()));

				if (MainMenuUserWidget)
				{
					MainMenuUserWidget->AddToViewport();

					MainMenuUserWidget->SetUp();
					GameInstance->RefreshFriendsList();
				}
			}
		}
	}
}

void AMenuController::SetMenuLoading()
{
	MainMenuUserWidget->SetLoading();
}

void AMenuController::ClearHUDWidgets_Implementation()
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
