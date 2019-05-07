// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameMode.h"
#include "Controllers/BDPlayerController.h"
#include "Pawns/PlayerChar.h"
#include "Main/BDGameState.h"
#include "Engine/World.h"
#include "Main/BDPlayerState.h"


APlayerController* ABDGameMode::ProcessClientTravel(FString& FURL, FGuid NextMapGuid, bool bSeamless, bool bAbsolute)
{
	// We call PreClientTravel directly on any local PlayerPawns (ie listen server)
	APlayerController* LocalPlayerController = NULL;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (Cast<UNetConnection>(PlayerController->Player) != NULL)
		{
			/* Check if we're using our Custom Controller */
			ABDPlayerController* MyController = Cast<ABDPlayerController>(PlayerController);
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
			ABDPlayerController* MyController = Cast<ABDPlayerController>(PlayerController);
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
