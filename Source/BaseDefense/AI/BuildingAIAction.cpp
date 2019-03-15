// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingAIAction.h"
#include "Building.h"

void UBuildingAIAction::Initialise(ABuilding* ABuilding)
{
	Building = ABuilding;
	BuildingData = &Building->BaseBuildingData;
}
