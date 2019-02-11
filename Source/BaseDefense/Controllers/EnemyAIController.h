// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyAIController.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(EnemyAIController, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveComplete, EPathFollowingResult::Type, PathResult);

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:

	AEnemyAIController(const FObjectInitializer& ObjectInitializer);




	virtual void Possess(class APawn* inPawn) override;
	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(
		const FAIRequestID RequestID,
		const FPathFollowingResult &Result) override;

	void SetAggroTarget();
	
	void WhatDo();

public:
	UPROPERTY()
	FOnMoveComplete OnMovementComplete;


	UPROPERTY(VisibleAnywhere)
	TArray<class UEnemyAIAction*> Actions;
	UPROPERTY()
	class UEnemyAIAction* CurrentAction = nullptr;

	UPROPERTY()
	class AActor* AggroTarget = nullptr;

	bool IsReloading = false;
	UPROPERTY()
	class AEnemyChar* MyChar = nullptr;
protected:
	UPROPERTY()
	class UWorld* World;
	class ABDGameState* GameState;
};
