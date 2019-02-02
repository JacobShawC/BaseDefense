// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIAction.h"
#include "EnemyAttackAIAction.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UEnemyAttackAIAction : public UEnemyAIAction
{
	GENERATED_BODY()
	
public:

	virtual void Initialise(class AEnemyAIController* AnOwner) override;
	/** Activate action. */
	virtual void Activate() override;

	/** Should we activate action this time ? */
	virtual void SetShouldActivate() override;

	void SetCurrentTarget();

	/** Abort action to start something else. */
	virtual void Abort() override;

public:
	UFUNCTION()
	void OnRangeOverlap(class UPrimitiveComponent* AComponent, AActor* AnActor, class UPrimitiveComponent* OtherComponent, int32 ABodyIndex, bool bFromSweep, const FHitResult& ASweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void FinishedAttack();
public:
	
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* RangeCapsule = nullptr;

	class AActor* CurrentTarget = nullptr;

	int Overlapping = 0;
};
