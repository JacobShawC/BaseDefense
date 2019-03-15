// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Main/StructLibrary.h"
#include "BDGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UBDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UBDGameInstance(const FObjectInitializer& ObjectInitializer);


public:
	TMap<FString, TSubclassOf<class UUserWidget>> Widgets;
	TMap<EBuilding, class UTexture2D*> Images;
	TMap<EBuilding, FBuildingData> Buildings;
	TMap<EBuilding, TArray<FBuildingBuffStruct>> BuildingsLevels;
	TMap<EBuilding, TArray<FBuildingBuffStruct>> BuildingUpgrades;
	TMap<EEnemy, FEnemyData> Enemies;
	FPlayerData DefaultPlayerData;
};
