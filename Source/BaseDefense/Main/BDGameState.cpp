// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameState.h"
#include "GameFramework/PlayerState.h"
#include "PlayerChar.h"
#include "BDPlayerState.h"
#include "Building.h"
#include "EnemyChar.h"
#include "Engine/World.h"
#include "GUI.h"
#include "MiniMap.h"
#include "BDPlayerController.h"


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

void ABDGameState::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
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
