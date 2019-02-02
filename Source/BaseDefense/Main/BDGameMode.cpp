// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameMode.h"
#include "Controllers/BDPlayerController.h"
#include "Pawns/PlayerChar.h"
#include "Main/BDGameState.h"
#include "Main/BDPlayerState.h"


ABDGameMode::ABDGameMode()
{
	GameStateClass = ABDGameState::StaticClass();
	DefaultPawnClass = APlayerChar::StaticClass();
	PlayerControllerClass = ABDPlayerController::StaticClass();
	PlayerStateClass = ABDPlayerState::StaticClass();
	bUseSeamlessTravel = true;

}

void ABDGameMode::PostLogin(APlayerController * NewPlayer)
{
	(NewPlayer->GetPlayerState<ABDPlayerState>())->SetUpTestData();

	Super::PostLogin(NewPlayer);
}

void ABDGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

}
