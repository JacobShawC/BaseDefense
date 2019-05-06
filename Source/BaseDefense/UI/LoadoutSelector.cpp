// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadoutSelector.h"
#include "Engine/World.h"
#include "BDGameInstance.h"
#include "BDGameState.h"
#include "PreBuilding.h"
#include "PreLevel.h"
#include "PreInfoSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "BDSaveGame.h"

void ULoadoutSelector::Setup()
{
	SetupBuildings();
}


void ULoadoutSelector::SetupBuildings()
{
	ABDGameState* GameState = GetWorld()->GetGameState<ABDGameState>();
	GameInstance = GetWorld() != NULL ? GetWorld()->GetGameInstance<UBDGameInstance>() : nullptr;

	if (GameState != nullptr)
	{
		GameState->LevelRewardsUpdated.AddUObject(this, &ULoadoutSelector::RefreshBuildings);
		GameState->LevelRewardsUpdated.AddUObject(this, &ULoadoutSelector::RefreshInformation);
	}

	if (GetOwningPlayerPawn()->Role == ROLE_Authority)
	{
		if (GameInstance != nullptr)
		{
			GameInstance->RefreshLevelRewards();
		}
	}

	//Set up Building Widgets
	TArray<UPreBuilding*> BuildingWidgets;
	TSubclassOf<UUserWidget>* PreBuildingClass = (GameInstance->Widgets).Find("PreBuilding");


	for (auto Iterator = GameInstance->Buildings.CreateConstIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Value.PreGameUnlockable)
		{
			UPreBuilding* PreBuildingWidget = Cast<UPreBuilding>(CreateWidget<UUserWidget>(this, PreBuildingClass->Get()));
			PreBuildingWidget->SetUp(Iterator->Value);
			PreBuildingWidget->OnSelfClicked.AddUObject(this, &ULoadoutSelector::PreBuildingClicked);
			BuildingWidgets.Add(PreBuildingWidget);
		}

	}

	//Insertion sort the levels based on unlock cost
	for (int i = 1; i < BuildingWidgets.Num(); i++)
	{
		for (int j = i; j > 0 && BuildingWidgets[j - 1]->BuildingData.PreGameUnlockCost > BuildingWidgets[j]->BuildingData.PreGameUnlockCost; j--)
		{
			BuildingWidgets.Swap(j, j - 1);
		}
	}

	//Slot width
	int Width = 3;
	for (int i = 1; i <= BuildingWidgets.Num(); i++)
	{
		float RowFloat = (float)i / (float)Width;
		int Row = ceil(RowFloat);
		int Column = i % 3;
		if (Column == 0) Column = Width;
		UUniformGridSlot * BuildingSlot = BuildingsGridPanel->AddChildToUniformGrid(BuildingWidgets[i - 1]);

		BuildingSlot->SetColumn(Column - 1);
		BuildingSlot->SetRow(Row - 1);
	}
}

void ULoadoutSelector::SetupBuildingInformation(FBuildingData ABuildingData)
{
	InformationSlotBox->ClearChildren();

	GameInstance = GetWorld() != NULL ? GetWorld()->GetGameInstance<UBDGameInstance>() : nullptr;
	TSubclassOf<UUserWidget>* PreInfoClass = GameInstance->Widgets.Find("PreInfo");

	InformationTitle->SetText(FText::FromString(ABuildingData.Name));
	InformationImage->SetBrushFromTexture(ABuildingData.Thumbnail);
	InformationText->SetText(FText::FromString(ABuildingData.Description));


	for (auto& ADifficulty : ABuildingData.LoadoutUpgrades)
	{
		UPreInfoSlot* InfoSlot = CreateWidget<UPreInfoSlot>(this, PreInfoClass->Get());

		InfoSlot->SetUp(ADifficulty.Key, ABuildingData);

		InformationSlotBox->AddChild(InfoSlot);
	}
}

void ULoadoutSelector::RefreshBuildings()
{
	ABDGameState* GameState = GetWorld()->GetGameState<ABDGameState>();

	TArray<UWidget*> Widgets = BuildingsGridPanel->GetAllChildren();
	int CurrentLevelReward = GameState->LevelRewards;

	for (auto AWidget : Widgets)
	{
		UPreBuilding* BuildingSlot = nullptr;

		BuildingSlot = Cast<UPreBuilding>(AWidget);

		if (BuildingSlot != nullptr)
		{
			if (CurrentLevelReward > BuildingSlot->BuildingData.PreGameUnlockCost)
			{
				BuildingSlot->SetLocked(false);
			}
			else
			{
				BuildingSlot->SetLocked(true);
			}
		}
	}
}

void ULoadoutSelector::RefreshInformation()
{
	TArray<UWidget*> Slots = InformationSlotBox->GetAllChildren();

	for (auto& ASlot : Slots)
	{
		UPreInfoSlot* InfoSlot = nullptr;
		InfoSlot = Cast<UPreInfoSlot>(ASlot);
		if (InfoSlot != nullptr)
		{
			InfoSlot->Refresh();
		}
	}
}


void ULoadoutSelector::PreBuildingClicked(UPreBuilding* ABuilding)
{
	if (ABuilding->BuildingData.Building != SelectedBuilding)
	{
		SelectedBuilding = ABuilding->BuildingData.Building;
		SetupBuildingInformation(ABuilding->BuildingData);
	}
	
}

void ULoadoutSelector::PreInfoSlotClicked(UPreInfoSlot* AnInfoSlot)
{

}

void ULoadoutSelector::OnStartButtonClicked()
{

}
