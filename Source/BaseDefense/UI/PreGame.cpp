// Fill out your copyright notice in the Description page of Project Settings.


#include "PreGame.h"
#include "Engine/World.h"
#include "BDGameInstance.h"
#include "BDGameState.h"
#include "PreBuilding.h"
#include "PreLevel.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "BDSaveGame.h"
#include "PreInfoSlot.h"

void UPreGame::Setup()
{
	InviteFriendsButton->OnClicked.AddDynamic(this, &UPreGame::OnInviteFriendsButtonClicked);
	
	if (GetOwningPlayerPawn()->Role == ROLE_Authority)
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

	if (GameState != nullptr)
	{
		GameState->LevelRewardsUpdated.AddUObject(this, &UPreGame::RefreshLevels);
		GameState->LevelRewardsUpdated.AddUObject(this, &UPreGame::RefreshInformation);
		GameState->SelectedLevelUpdated.AddUObject(this, &UPreGame::RefreshSelectedLevel);
		GameState->SelectedLevelDifficultyUpdated.AddUObject(this, &UPreGame::RefreshSelectedLevel);
	}

	
	if (GetOwningPlayerPawn()->Role == ROLE_Authority)
	{
		if (GameInstance != nullptr)
		{
			GameInstance->RefreshLevelRewards();
		}
	}

	//Set up Level Widgets
	TArray<UPreLevel*> LevelWidgets;
	TSubclassOf<UUserWidget>* PreLevelClass = (GameInstance->Widgets).Find("PreLevel");

	for (auto Iterator = GameInstance->Levels.CreateConstIterator(); Iterator; ++Iterator)
	{
		UPreLevel* PreLevelWidget = Cast<UPreLevel>(CreateWidget<UUserWidget>(this, PreLevelClass->Get()));
		PreLevelWidget->SetUp(Iterator->Value);
		PreLevelWidget->OnSelfClicked.AddUObject(this, &UPreGame::PreLevelClicked);

		LevelWidgets.Add(PreLevelWidget);
	}

	//Insertion sort the levels based on unlock cost
	for (int i = 1; i < LevelWidgets.Num(); i++)
	{
		for (int j = i; j > 0 && LevelWidgets[j - 1]->LevelData.PreGameUnlockCost > LevelWidgets[j]->LevelData.PreGameUnlockCost; j--)
		{
			LevelWidgets.Swap(j, j - 1);
		}
	}

	//Slot width
	int LevelGridWidth = 3;
	for (int i = 1; i <= LevelWidgets.Num(); i++)
	{
		float RowFloat = (float)i / (float)LevelGridWidth;
		int Row = ceil(RowFloat);
		int Column = i % 3;
		if (Column == 0) Column = LevelGridWidth;
		UUniformGridSlot* LevelSlot = LevelsGridPanel->AddChildToUniformGrid(LevelWidgets[i-1]);
		
		LevelSlot->SetColumn(Column - 1);
		LevelSlot->SetRow(Row - 1);
	}



	RefreshSelectedLevel();
	RefreshLevels();
}

void UPreGame::RefreshLevels()
{
	ABDGameState* GameState = GetWorld()->GetGameState<ABDGameState>();

	TArray<UWidget*> Widgets = (LevelsGridPanel->GetAllChildren());

	int CurrentLevelReward = GameState->LevelRewards;

	for (auto AWidget : Widgets)
	{
		UPreLevel* ALevel = Cast<UPreLevel>(AWidget);

		if (ALevel != nullptr)
		{
			if (CurrentLevelReward > ALevel->LevelData.PreGameUnlockCost)
			{
				ALevel->SetLocked(false);
			}
			else
			{
				ALevel->SetLocked(true);
			}
		}
	}
}

void UPreGame::SetUpLevelInformation(FLevelData ALevelData)
{
	GameInstance = GetWorld() != NULL ? GetWorld()->GetGameInstance<UBDGameInstance>() : nullptr;
	TSubclassOf<UUserWidget>* InfoSlotClass = GameInstance->Widgets.Find("PreInfoSlot");

	InformationTitle->SetText(FText::FromString(ALevelData.Name));
	InformationImage->SetBrushFromTexture(ALevelData.MiniMap);
	InformationText->SetText(FText::FromString(ALevelData.Description));

	InformationSlotBox->ClearChildren();
	for (auto& ADif : ALevelData.DifficultyRewards)
	{
		

		UPreInfoSlot* InfoSlot = Cast<UPreInfoSlot>(CreateWidget<UUserWidget>(this, InfoSlotClass->Get()));
		InfoSlot->SetUp(ADif.Key, ALevelData);
		InformationSlotBox->AddChild(InfoSlot);

		if (GetOwningPlayerPawn()->Role == ROLE_Authority)
		{
			InfoSlot->OnSelfClicked.AddUObject(this, &UPreGame::PreInfoSlotClicked);
		}
		else
		{
			InfoSlot->SetRenderOpacity(0.5f);
		}

	}
		
}

void UPreGame::RefreshInformation()
{


}

void UPreGame::RefreshLevelRewards()
{
	
}

void UPreGame::RefreshSelectedLevel()
{
	GameInstance = GetWorld() != NULL ? GetWorld()->GetGameInstance<UBDGameInstance>() : nullptr;

	ABDGameState* GameState = GetWorld()->GetGameState<ABDGameState>();

	if (GameState->SelectedLevel == ELevel::None || GameState->SelectedLevelDifficulty == ELevelDifficulty::None) return;

	ELevel ALevel = GameState->SelectedLevel;

	FString LevelName = GameInstance->Levels.Find(ALevel)->Name;
	ELevelDifficulty ADifficulty = GameState->SelectedLevelDifficulty;
	
	FString DifficultyText = "";
	switch (ADifficulty) {
	case ELevelDifficulty::None: DifficultyText = "None";
		break;
	case ELevelDifficulty::Easy: DifficultyText = "Easy";
		break;
	case ELevelDifficulty::Medium: DifficultyText = "Medium";
		break;
	case ELevelDifficulty::Hard: DifficultyText = "Hard";
		break;
	case ELevelDifficulty::Brutal: DifficultyText = "Brutal";
		break;
	case ELevelDifficulty::Survival: DifficultyText = "Survival";
		break;
	case ELevelDifficulty::Challenge1: DifficultyText = "Challenge1";
		break;
	case ELevelDifficulty::Challenge2: DifficultyText = "Challenge2";
		break;
	}
	//Set level name
	SelectedLevelText->SetText(FText::FromString(LevelName));

	SelectedDifficultyText->SetText(FText::FromString(DifficultyText));
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
}

void UPreGame::PreLevelClicked(UPreLevel* ALevel)
{
	if (SelectedLevel != ALevel->LevelData.Level)
	{
		SetUpLevelInformation(ALevel->LevelData);
	}
}

void UPreGame::PreInfoSlotClicked(UPreInfoSlot* AnInfoSlot)
{
	if (GetOwningPlayerPawn()->Role == ROLE_Authority)
	{
		ABDGameState* GameState = GetWorld()->GetGameState<ABDGameState>();

		GameState->SelectedLevel = AnInfoSlot->LevelData.Level;
		GameState->SelectedLevelDifficulty = AnInfoSlot->LevelDifficulty;
		GameState->OnRep_SelectedLevel();
	}
}

