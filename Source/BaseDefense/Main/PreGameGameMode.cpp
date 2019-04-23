// Fill out your copyright notice in the Description page of Project Settings.


#include "PreGameGameMode.h"
#include "Controllers/PreGameController.h"
#include "Pawns/MenuPawn.h"
#include "Main/BDGameState.h"
#include "Main/BDPlayerState.h"

APreGameGameMode::APreGameGameMode()
{
	GameStateClass = ABDGameState::StaticClass();
	DefaultPawnClass = AMenuPawn::StaticClass();
	PlayerControllerClass = APreGameController::StaticClass();
	PlayerStateClass = ABDPlayerState::StaticClass();
}
