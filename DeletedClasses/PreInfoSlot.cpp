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
#include "GameFramework/PlayerController.h"
#include "PreGame.h"

void UPreInfoSlot::SetUp(ELevelDifficulty ADifficulty, FLevelData AData)
{
	LevelDifficulty = ADifficulty;
	LevelData = AData;
	LevelOrBuilding = true;
	SlotButton->OnClicked.AddDynamic(this, &UPreInfoSlot::OnButtonClicked);

	SlotButtonText->SetText(FText::FromString("Select Level"));
	FString DifficultyText = "";
	switch (ADifficulty) {
	case ELevelDifficulty::None: DifficultyText = "None";
		break;
	case ELevelDifficulty::Easy : DifficultyText = "Easy";
		break;
	case ELevelDifficulty::Medium : DifficultyText = "Medium";
		break;
	case ELevelDifficulty::Hard : DifficultyText = "Hard";
		break;
	case ELevelDifficulty::Brutal : DifficultyText = "Brutal";
		break;
	case ELevelDifficulty::Survival : DifficultyText = "Survival";
		break;
	case ELevelDifficulty::Challenge1 : DifficultyText = "Challenge1";
		break;
	case ELevelDifficulty::Challenge2: DifficultyText = "Challenge2";
		break;
	}

	SlotTitle->SetText(FText::FromString(DifficultyText));
	SlotDescription->SetText(FText::FromString(LevelData.Description));

	Refresh();
}

void UPreInfoSlot::SetUp(EBuildingUpgrade AnUpgrade, FBuildingData AData)
{
	BuildingData = AData;
	LevelOrBuilding = false;
	BuildingUpgrade = AnUpgrade;
	SlotButton->OnClicked.AddDynamic(this, &UPreInfoSlot::OnButtonClicked);
	Refresh();
}

void UPreInfoSlot::OnButtonClicked()
{
	OnSelfClicked.Broadcast(this);
}

void UPreInfoSlot::Refresh()
{
	
}

void UPreInfoSlot::SetLocked(bool ALocked)
{
	if (ALocked)
	{
		SetRenderOpacity(0.5f);
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	}
	else
	{
		SetRenderOpacity(1.0f);
		SetVisibility(ESlateVisibility::Visible);

	}
}
