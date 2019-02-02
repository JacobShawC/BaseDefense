// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIAction.h"
#include "BuildingAIAction.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UBuildingAIAction : public UAIAction
{
	GENERATED_BODY()
	
public:

	/** Activate action. */
	virtual void Initialise(class ABuilding* ABuilding);

public:

	class ABuilding* Building = nullptr;

	FBuildingData* BuildingData = nullptr;
};
