// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameState.h"
#include "GameFramework/PlayerState.h"
#include "PlayerChar.h"
#include "BDPlayerState.h"
#include "BDPlayerController.h"


void ABDGameState::AddMoney(float AMoney)
{
	if (!PlayerArray.Num() == 0)
	{
		float DividedMoney = AMoney / PlayerArray.Num();
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
