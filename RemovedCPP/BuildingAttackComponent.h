// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AttackComponent.h"
#include "BuildingAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UBuildingAttackComponent : public UAttackComponent
{
	GENERATED_BODY()

public:
	void Initialise(FBuildingData ABuildingData, AActor* AnOwner);

	UFUNCTION()
	void OnRangeOverlap(UPrimitiveComponent* AComponent, AActor* AnActor, UPrimitiveComponent* OtherComponent, int32 ABodyIndex, bool bFromSweep, const FHitResult& ASweepResult);

public:

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* RangeCapsule = nullptr;

	UPROPERTY(VisibleAnywhere)
	AActor* Owner = nullptr;

	UPROPERTY(VisibleAnywhere)
	FBuildingData BuildingData;

};
