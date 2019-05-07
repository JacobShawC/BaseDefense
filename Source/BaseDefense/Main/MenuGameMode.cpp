// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "Controllers/MenuController.h"
#include "Pawns/MenuPawn.h"
#include "Main/BDGameState.h"
#include "Engine/World.h"

#include "Main/BDPlayerState.h"

AMenuGameMode::AMenuGameMode()
{
	GameStateClass = ABDGameState::StaticClass();
	DefaultPawnClass = AMenuPawn::StaticClass();
	PlayerControllerClass = AMenuController::StaticClass();
	PlayerStateClass = ABDPlayerState::StaticClass();
	bUseSeamlessTravel = true;
}

APlayerController* AMenuGameMode::ProcessClientTravel(FString& FURL, FGuid NextMapGuid, bool bSeamless, bool bAbsolute)
{
	// We call PreClientTravel directly on any local PlayerPawns (ie listen server)
	APlayerController* LocalPlayerController = NULL;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (Cast<UNetConnection>(PlayerController->Player) != NULL)
		{
			/* Check if we're using our Custom Controller */
			AMenuController* MyController = Cast<AMenuController>(PlayerController);
			if (MyController)
			{
				MyController->ClearHUDWidgets();
			}

			// REMOTE PLAYER
			PlayerController->ClientTravel(FURL, TRAVEL_Relative, bSeamless, NextMapGuid);
		}
		else
		{
			// LOCAL PLAYER
			/* Check if we're using a GES Controller */
			AMenuController* MyController = Cast<AMenuController>(PlayerController);
			if (MyController)
			{
				MyController->ClearHUDWidgets();
			}

			LocalPlayerController = PlayerController;
			PlayerController->PreClientTravel(FURL, bAbsolute ? TRAVEL_Absolute : TRAVEL_Relative, bSeamless);
		}
	}
	return LocalPlayerController;
}
