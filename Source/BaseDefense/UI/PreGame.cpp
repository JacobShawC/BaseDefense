// Fill out your copyright notice in the Description page of Project Settings.


#include "PreGame.h"
#include "Engine/World.h"
#include "BDGameInstance.h"
#include "BDGameState.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "BDSaveGame.h"

void UPreGame::Setup()
{
	InviteFriendsButton->OnClicked.AddDynamic(this, &UPreGame::OnInviteFriendsButtonClicked);
	
	if (GetOwningPlayerPawn()->GetLocalRole() == ROLE_Authority)
	{
		StartButton->OnClicked.AddDynamic(this, &UPreGame::OnStartButtonClicked);
	}
	else
	{
		StartButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	SettingsButton->OnClicked.AddDynamic(this, &UPreGame::OnSettingsButtonClicked);
	LeaveButton->OnClicked.AddDynamic(this, &UPreGame::OnLeaveButtonClicked);

	ABDGameState* GameState = GetWorld()->GetGameState<ABDGameState>();
	GameInstance = GetWorld() != NULL ? GetWorld()->GetGameInstance<UBDGameInstance>() : nullptr;
	
}

void UPreGame::OnStartButtonClicked()
{
	ABDGameState* GameState = GetWorld()->GetGameState<ABDGameState>();

	GameInstance = GetWorld() != NULL ? GetWorld()->GetGameInstance<UBDGameInstance>() : nullptr;
	if (GameState->SelectedLevel != ELevel::None)
	{
		GameInstance->LoadLevel(GameState->SelectedLevel);
	}
}

void UPreGame::OnInviteFriendsButtonClicked()
{
}

void UPreGame::OnSettingsButtonClicked()
{
}

void UPreGame::OnLeaveButtonClicked()
{
	GameInstance = GetWorld() != NULL ? GetWorld()->GetGameInstance<UBDGameInstance>() : nullptr;
	if (GameInstance != nullptr)
	{
		GameInstance->LoadMainMenu();

	}
}

