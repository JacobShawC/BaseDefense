// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameState.h"
#include "GameFramework/PlayerState.h"
#include "PlayerChar.h"
#include "BDPlayerState.h"
#include "BDGameInstance.h"
#include "BDSaveGame.h"
#include "Building.h"
#include "EnemyChar.h"
#include "Engine/World.h"
#include "GUI.h"
#include "MiniMap.h"
#include "BDPlayerController.h"
#include "BaseLevel.h"
#include "Level1.h"
#include "BDGameInstance.h"
#include "StructLibrary.h"
#include "BDPlayerState.h"
#include "LevelGeneration.h"
#include "Public/KismetAnimationLibrary.h"
#include <GameFramework/Actor.h>
#include "UnitManager.h"


void ABDGameState::GetMiniMap()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		ABDPlayerController* Controller = Cast<ABDPlayerController>(World->GetFirstPlayerController());
	
		if (Controller != nullptr)
		{
			UGUI* GUI = Controller->GUIWidget;

			if (GUI != nullptr)
			{
				MiniMap = GUI->MiniMap;
			}
		}
	}
}

float ABDGameState::AddMoney(float AMoney)
{
	float DividedMoney = 0.0f;
	{
		DividedMoney = AMoney / PlayerArray.Num();
		for (APlayerState* AState : PlayerArray)
		{
			ABDPlayerState* ABDState = Cast<ABDPlayerState>(AState);
			if (ABDState)
			{
				ABDState->Money += DividedMoney;
				ABDState->OnRep_Money();
			}
		}
	}
	return DividedMoney;
}

void ABDGameState::SetCurrentState(EGameState AState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentState = AState;

		if (SelectedLevel == ELevel::None)
		{
			SelectedLevel = ELevel::Level1;
		}

		if (CurrentState == EGameState::InProgress)
		{
			StartGame();
		}
	}
}

void ABDGameState::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		LevelGenerationActor = (ALevelGeneration*)GetWorld()->SpawnActor<ALevelGeneration>(ALevelGeneration::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		OnRep_LevelGenerationActor();

		UnitManagerActor = (AUnitManager*)GetWorld()->SpawnActor<AUnitManager>(AUnitManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);


		for (APlayerState* AState : PlayerArray)
		{
			ABDPlayerState* ABDState = Cast<ABDPlayerState>(AState);
			if (ABDState)
			{
				ABDState->OnRep_Money();
			}
		}
	}
}

void ABDGameState::AddBuilding(TWeakObjectPtr<ABuilding> ABuilding)
{
	Buildings.AddUnique(ABuilding);
	if (MiniMap == nullptr || !MiniMap->IsValidLowLevel())
	{
		GetMiniMap();
	}

	if (MiniMap != nullptr)
	{
		MiniMap->AddBuilding(ABuilding);
	}

}

void ABDGameState::AddEnemyCharacter(TWeakObjectPtr<AEnemyChar> AnEnemyChar)
{
	EnemyCharacters.AddUnique(AnEnemyChar);
	if (MiniMap == nullptr || !MiniMap->IsValidLowLevel())
	{
		GetMiniMap();
	}

	if (MiniMap != nullptr)
	{
		MiniMap->AddEnemyCharacter(AnEnemyChar);
	}
}


void ABDGameState::SetMapProperties(FVector APosition, float AWidth)
{
	OrthoWidth = AWidth;
	MapCapturePosition = FVector2D(APosition);
	if (MiniMap == nullptr || !MiniMap->IsValidLowLevel())
	{
		GetMiniMap();
	}

	if (MiniMap != nullptr)
	{
		MiniMap->MapPosition = FVector2D(APosition);
		MiniMap->MapWidth = AWidth;
	}
}

void ABDGameState::RefreshLevelRewards()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UBDGameInstance* GameInstance = GetWorld() != NULL ? GetWorld()->GetGameInstance<UBDGameInstance>() : nullptr;
		if (GameInstance != nullptr)
		{
			GameInstance->RefreshLevelRewards();
			if (GameInstance->CurrentSave != nullptr)
			{
				LevelRewards = GameInstance->CurrentSave->Points;
			}
		}
	}
}

EGameState ABDGameState::GetCurrentState()
{
	return CurrentState;
}

TArray<APlayerChar*> ABDGameState::GetPlayerPawns()
{
	TArray<APlayerChar*> PlayerCharacters;
	for (APlayerState* AState : PlayerArray)
	{
		APlayerChar* Character = nullptr;
		ABDPlayerController* OwnerController = Cast<ABDPlayerController>(AState->GetOwner());
		Character = Cast<APlayerChar>(OwnerController->GetPawn());
		if (Character)
		{
			PlayerCharacters.Add(Character);
		}
	}
	return PlayerCharacters;
}

void ABDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABDGameState, LevelRewards);
	DOREPLIFETIME(ABDGameState, SelectedLevel);
	DOREPLIFETIME(ABDGameState, SelectedLevelDifficulty);
	DOREPLIFETIME(ABDGameState, LevelSaves);
	DOREPLIFETIME(ABDGameState, CurrentState);
	DOREPLIFETIME(ABDGameState, LevelGenerationActor);
	DOREPLIFETIME(ABDGameState, UnitManagerActor);
}
void ABDGameState::OnRep_LevelRewards()
{
	LevelRewardsUpdated.Broadcast();
}

void ABDGameState::OnRep_LevelSaves()
{
	LevelSavesUpdated.Broadcast();
}

void ABDGameState::OnRep_CurrentState()
{
	CurrentStateUpdated.Broadcast();
}

void ABDGameState::OnRep_SelectedLevel()
{
	SelectedLevelUpdated.Broadcast();
}

void ABDGameState::OnRep_SelectedLevelDifficulty()
{
	SelectedLevelDifficultyUpdated.Broadcast();
}

void ABDGameState::OnRep_LevelGenerationActor()
{
	LevelGenerationActorUpdated.Broadcast();
}

void ABDGameState::SetLevelRewards(int ALevelRewards)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		LevelRewards = ALevelRewards;
		OnRep_LevelRewards();
	}
}

void ABDGameState::StartGame()
{
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	FLevelData LevelData;
	UBDGameInstance* GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		LevelData = *GameInstance->Levels.Find(SelectedLevel);
	}
	

	if (SelectedLevel == ELevel::Level1)
	{
		Cast<ABaseLevel>(GetWorld()->SpawnActor<ALevel1>());
	}

	AddMoney(LevelData.StartingGold);

}
