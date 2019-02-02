// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BDGameState.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API ABDGameState : public AGameState
{
	GENERATED_BODY()
public:
	TArray<class APlayerChar*> GetPlayerPawns();
};
