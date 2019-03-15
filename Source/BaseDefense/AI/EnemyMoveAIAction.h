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
	
	UFUNCTION()
	void CurrentTargetDestroyed();

	UFUNCTION()
	void OnAttacked(AActor* AttackingTarget, float ADamage);

public:

	UPROPERTY()
	class AActor* CurrentTarget = nullptr;
	UPROPERTY()
	class AActor* DefaultTarget = nullptr;

	TArray<AActor*> TargetsChecked;

	float ShortestTarget = 0;

protected:
	UPROPERTY()
	class ABDGameState* GameState = nullptr;

	class UNavigationSystemV1* NavSys;
};
