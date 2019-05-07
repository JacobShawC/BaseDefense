// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API AMenuGameMode : public AGameMode
{
	GENERATED_BODY()
		AMenuGameMode();
public:
	virtual APlayerController* ProcessClientTravel(FString& FURL, FGuid NextMapGuid, bool bSeamless, bool bAbsolute) override;

};
