// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingAIAction.h"
#include "BuildingAttackAIAction.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UBuildingAttackAIAction : public UBuildingAIAction
{
	GENERATED_BODY()
	virtual void Initialise(class ABuilding* ABuilding) override;
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

	UPROPERTY()
	class AActor* CurrentTarget = nullptr;

	int Overlapping = 0;

	UPROPERTY(VisibleAnywhere)
	class UBuildingAIAction* CurrentAction = nullptr;
};
