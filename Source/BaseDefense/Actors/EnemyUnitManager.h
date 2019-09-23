// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnitManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HISMManager.h"
#include "StructLibrary.h"
#include "EnemyUnitManager.generated.h"



USTRUCT()
struct FEnemyUnitData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		EEnemy UnitType = EEnemy::None;

	UPROPERTY()
		uint32 UnitID;

	UPROPERTY()
		class USphereComponent* Sphere = nullptr;

};

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API AEnemyUnitManager : public AUnitManager
{
	GENERATED_BODY()
	
public:
	uint32 SpawnEnemyUnit(EEnemy AUnit, FTransform AnInitialTransform);
	void TestSpawn();
	virtual void BeginPlay() override;

private:
	AEnemyUnitManager();

	void UpdateHISMPositions();
	void ForceUnit(uint32 AUnitID, FVector Direction, float AnAmount);

private:
	TMap<EEnemy, TArray<FEnemyUnitData>> EnemyTypeMap;
	TMap<EEnemy, FEnemyUnitData> EnemyDataMap;
	TMap<uint32, FEnemyUnitData> UnitIDMap;

	UPROPERTY()
	TMap<EEnemy, AHISMManager*> HISMManagers;
};
