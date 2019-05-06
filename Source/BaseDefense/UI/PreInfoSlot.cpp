// Fill out your copyright notice in the Description page of Project Settings.


#include "PreInfoSlot.h"
#include "PreLevel.h"
#include "PreBuilding.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "StructLibrary.h"
#include "Engine/World.h"
#include "BDGameState.h"
#include "Engine/World.h"
#include "BDPlayerController.h"
#include "BDPlayerState.h"
#include "PlayerChar.h"
#include "PreGame.h"

void UPreInfoSlot::SetUp(ELevelDifficulty ADifficulty, FLevelData AData)
{
	LevelDifficulty = ADifficulty;
	LevelData = AData;
	LevelOrBuilding = true;
	SlotButton->OnClicked.AddDynamic(this, &UPreInfoSlot::OnButtonClicked);

	SlotButtonText->SetText(FText::FromString("Select Level"));

	SlotTitle->SetText(FText::FromString(LevelData.Name));
	SlotDescription->SetText(FText::FromString(LevelData.Description));


	Refresh();
}

void UPreInfoSlot::SetUp(EBuildingUpgrade AnUpgrade, FBuildingData AData)
{
	BuildingData = AData;
	LevelOrBuilding = false;
	SlotButton->OnClicked.AddDynamic(this, &UPreInfoSlot::OnButtonClicked);
}

void UPreInfoSlot::OnButtonClicked()
{
	OnSelfClicked.Broadcast(this);
}

void UPreInfoSlot::Refresh()
{
	ABDGameState* GameState = GetOwningPlayer<ABDPlayerController>()->GetWorld()->GetGameState<ABDGameState>();

	ABDPlayerState* PlayerState = GetOwningPlayerPawn<APlayerChar>()->GetPlayerState<ABDPlayerState>();

	if (GameState == nullptr || PlayerState == nullptr)
		return;

	if (LevelOrBuilding)
	{
		if (GameState != nullptr)
		{
			if (GameState->LevelRewards > LevelData.PreGameUnlockCost)
			{
				SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				SetRenderOpacity(0.5f);
			}
			else
			{
				SetVisibility(ESlateVisibility::Visible);
				SetRenderOpacity(1.0f);
			}
		}
	}
	else
	{
		if (BuildingData.PreGameUnlockCost > PlayerState->RemainingLevelRewards
			|| PlayerState->Loadout.Buildings.Contains(BuildingData.Building)
			|| GameState->LevelRewards < BuildingData.PreGameUnlockCost
			|| !BuildingData.PreGameUnlockable)
		{
			SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SetRenderOpacity(0.5f);
		}
		else
		{
			SetVisibility(ESlateVisibility::Visible);
			SetRenderOpacity(1.0f);
		}
	}
}
