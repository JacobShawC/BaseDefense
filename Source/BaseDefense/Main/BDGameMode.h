// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BDGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API ABDGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual APlayerController* ProcessClientTravel(FString& FURL, FGuid NextMapGuid, bool bSeamless, bool bAbsolute) override;

protected:
	ABDGameMode();

	void PostLogin(class APlayerController* NewPlayer) override;
	void Logout(class AController* Exiting) override;
};
