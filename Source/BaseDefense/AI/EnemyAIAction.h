// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIAction.h"
#include "EnemyAIAction.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UEnemyAIAction : public UAIAction
{
	GENERATED_BODY()
public:

	/** Activate action. */
	virtual void Initialise(class AEnemyAIController* AController);

public:

	class AEnemyChar* Char = nullptr;
	class AEnemyAIController* AIController = nullptr;

	FEnemyData* EnemyData = nullptr;
};
