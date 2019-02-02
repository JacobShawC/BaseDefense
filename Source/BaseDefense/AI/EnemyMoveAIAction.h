// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/AITypes.h"
#include "EnemyAIAction.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyMoveAIAction.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UEnemyMoveAIAction : public UEnemyAIAction
{
	GENERATED_BODY()
	
public:
	virtual void Initialise(class AEnemyAIController* AnOwner) override;

	virtual void Activate() override;

	/** Abort action to start something else. */
	virtual void Abort() override;

	UFUNCTION()
	void MoveComplete(EPathFollowingResult::Type PathResult);

public:
	class APlayerChar* GetRandomPlayer();

	UFUNCTION()
	void SetTarget();

public:

	class AActor* CurrentTarget = nullptr;
	class AActor* DefaultTarget = nullptr;

protected:
	class ABDGameState* GameState = nullptr;
};
