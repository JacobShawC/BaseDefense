// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HISMManager.h"
#include "StructLibrary.h"
#include "UnitManager.generated.h"



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

UCLASS()
class BASEDEFENSE_API AUnitManager : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AUnitManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	uint32 UnitIDCount = 0;
	void SpawnEnemyUnit(EEnemy AUnit, FTransform AnInitialTransform);
	
	void ForceUnit(uint32 AUnitID, FVector Direction, float AnAmount);

	void TestSpawn();

	TMap<uint32, FEnemyUnitData> UnitIDMap;

	TMap<EEnemy, TArray<FEnemyUnitData*>> EnemyTypeMap;
	TMap<EEnemy, FEnemyUnitData> EnemyDataMap;



protected:
	class ALevelGeneration* LevelGenerationActor = nullptr;
public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TMap<EEnemy, AHISMManager*> HISMManagers;

};
