// Fill out your copyright notice in the Description page of Project Settings.


#include "BDSaveGame.h"
#include "BDGameInstance.h"
#include "Engine/World.h"

UBDSaveGame::UBDSaveGame()
{
	
}

int UBDSaveGame::RefreshPoints()
{
	UBDGameInstance* GameInstance = GetWorld()->GetGameInstance<UBDGameInstance>();

	int ReturnPoints = 0;

	for (auto It = LevelSaves.CreateConstIterator(); It; It++)
	{
		FLevelSave ASave = It->Value;
		FLevelData* LevelData = GameInstance->Levels.Find(It->Key);
		if (LevelData != nullptr)
		{
			ReturnPoints += LevelData->GetReward(ASave.ChallengesCompleted);
		}
	}


	Points = ReturnPoints;
	return Points;
}
