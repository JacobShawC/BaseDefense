// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameState.h"
#include "GameFramework/PlayerState.h"
#include "PlayerChar.h"
#include "BDPlayerController.h"


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
