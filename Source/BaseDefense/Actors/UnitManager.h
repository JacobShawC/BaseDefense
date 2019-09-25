// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HISMManager.h"
#include "StructLibrary.h"
#include "UnitManager.generated.h"



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
	void SpawnUnit(EGameUnit AUnit, FTransform AnInitialTransform);

	void ForceUnit(uint32 AUnitID, FVector Direction, float AnAmount);

	void SetupAllHISMS();

	void SetupHISM(FUnitData AUnitData);

	void PerformActions();
	void SetActionToDefault(FUnitInstance* AUnitInstance);

	void PathTowardsPosition(FUnitInstance AnInstance);
	UFUNCTION()
	void OnRangeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void TestSpawn();

	void Tick(float DeltaTime);

	void UpdateHISMPositions();

	TMap<uint32, FUnitInstance> UnitIDMap;

	TMap<EGameUnit, TArray<FUnitInstance>> UnitTypeMap;
	TMap<EGameUnit, FUnitData> UnitDataMap;



protected:
	class ALevelGeneration* LevelGenerationActor = nullptr;
public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		TMap<EGameUnit, AHISMManager*> HISMManagers;

};