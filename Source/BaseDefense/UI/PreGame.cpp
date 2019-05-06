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
#include "Components/Image.h"
#include "BDSaveGame.h"

void UPreGame::Setup()
{
	InviteFriendsButton->OnClicked.AddDynamic(this, &UPreGame::OnInviteFriendsButtonClicked);
	LevelsButton->OnClicked.AddDynamic(this, &UPreGame::OnLevelsButtonClicked);
	BuildingsButton->OnClicked.AddDynamic(this, &UPreGame::OnBuildingsButtonClicked);
	StartButton->OnClicked.AddDynamic(this, &UPreGame::OnStartButtonClicked);
	SettingsButton->OnClicked.AddDynamic(this, &UPreGame::OnSettingsButtonClicked);
	LeaveButton->OnClicked.AddDynamic(this, &UPreGame::OnLeaveButtonClicked);

	ABDGameState* GameState = GetWorld()->GetGameState<ABDGameState>();
	GameInstance = GetWorld() != NULL ? GetWorld()->GetGameInstance<UBDGameInstance>() : nullptr;

	if (GameState != nullptr)
	{
		GameState->LevelRewardsUpdated.AddUObject(this, &UPreGame::RefreshLevels);
		GameState->LevelRewardsUpdated.AddUObject(this, &UPreGame::RefreshBuildings);
		GameState->LevelRewardsUpdated.AddUObject(this, &UPreGame::RefreshInformation);
	}

	
	if (GetOwningPlayerPawn()->Role == ROLE_Authority)
	{
		if (GameInstance != nullptr)
		{
			GameInstance->RefreshLevelRewards();
		}
	}

	////Set up Level Widgets
	//TArray<UPreLevel*> LevelWidgets;
	//TSubclassOf<UUserWidget>* PreLevelClass = (GameInstance->Widgets).Find("PreLevel");

	//for (auto Iterator = GameInstance->Levels.CreateConstIterator(); Iterator; ++Iterator)
	//{
	//	UPreLevel* PreLevelWidget = Cast<UPreLevel>(CreateWidget<UUserWidget>(this, PreLevelClass->Get()));
	//	PreLevelWidget->SetUp(Iterator->Value, this);
	//	LevelWidgets.Add(PreLevelWidget);
	//}

	////Insertion sort the levels based on unlock cost
	//for (int i = 1; i < LevelWidgets.Num(); i++)
	//{
	//	for (int j = i; j > 0 && LevelWidgets[j - 1]->LevelData.PreGameUnlockCost > LevelWidgets[j]->LevelData.PreGameUnlockCost; j--)
	//	{
	//		LevelWidgets.Swap(j, j - 1);
	//	}
	//}

	////Slot width
	//int LevelGridWidth = 3;
	//for (int i = 1; i <= LevelWidgets.Num(); i++)
	//{
	//	float RowFloat = (float)i / (float)LevelGridWidth;
	//	int Row = ceil(RowFloat);
	//	int Column = i % 3;
	//	if (Column == 0) Column = LevelGridWidth;
	//	UUniformGridSlot* LevelSlot = LevelsGridPanel->AddChildToUniformGrid(LevelWidgets[i-1]);
	//	
	//	LevelSlot->SetColumn(Column - 1);
	//	LevelSlot->SetRow(Row - 1);
	//}




	////Set up Building Widgets
	//TArray<UPreBuilding*> BuildingWidgets;
	//TSubclassOf<UUserWidget>* PreBuildingClass = (GameInstance->Widgets).Find("PreBuilding");


	//for (auto Iterator = GameInstance->Buildings.CreateConstIterator(); Iterator; ++Iterator)
	//{
	//	if (Iterator->Value.PreGameUnlockable)
	//	{
	//		UPreBuilding* PreBuildingWidget = Cast<UPreBuilding>(CreateWidget<UUserWidget>(this, PreBuildingClass->Get()));
	//		PreBuildingWidget->SetUp(Iterator->Value, this);
	//		BuildingWidgets.Add(PreBuildingWidget);
	//	}
	//	
	//}

	////Insertion sort the levels based on unlock cost
	//for (int i = 1; i < BuildingWidgets.Num(); i++)
	//{
	//	for (int j = i; j > 0 && BuildingWidgets[j - 1]->BuildingData.PreGameUnlockCost > BuildingWidgets[j]->BuildingData.PreGameUnlockCost; j--)
	//	{
	//		BuildingWidgets.Swap(j, j - 1);
	//	}
	//}

	////Slot width
	//int Width = 3;
	//for (int i = 1; i <= BuildingWidgets.Num(); i++)
	//{
	//	float RowFloat = (float)i / (float)Width;
	//	int Row = ceil(RowFloat);
	//	int Column = i % 3;
	//	if (Column == 0) Column = Width;
	//	UUniformGridSlot* BuildingSlot = BuildingsGridPanel->AddChildToUniformGrid(BuildingWidgets[i-1]);
	//	
	//	BuildingSlot->SetColumn(Column - 1);
	//	BuildingSlot->SetRow(Row - 1);
	//}

}

void UPreGame::RefreshLevels()
{

	/*if (Iterator->Value.PreGameUnlockCost < GameState->LevelRewards)
	{

	}*/

}

void UPreGame::RefreshBuildings()
{
}

void UPreGame::RefreshInformation()
{
}

void UPreGame::RefreshLevelRewards()
{
	
}

void UPreGame::OnLevelsButtonClicked()
{
	if (SelectionSwitcher != nullptr)
	{
		SelectionSwitcher->SetActiveWidgetIndex(0);
	}
}

void UPreGame::OnBuildingsButtonClicked()
{
	if (SelectionSwitcher != nullptr)
	{
		SelectionSwitcher->SetActiveWidgetIndex(1);
	}
}

void UPreGame::OnStartButtonClicked()
{
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
}

void UPreGame::PreBuildingClicked(UPreBuilding* ABuilding)
{

}
