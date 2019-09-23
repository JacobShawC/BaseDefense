// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUnitManager.h"
#include <Components/SphereComponent.h>
#include "UnitManager.h"
#include "HISMManager.h"
#include "LevelGeneration.h"
// Sets default values
AEnemyUnitManager::AEnemyUnitManager()
{
	PrimaryActorTick.bCanEverTick = true;
	FEnemyUnitData SlowZombieData;
	//SlowZombieData.
	//EnemyDataMap
}

//We go though every unit's sphere and update the instances position via their HISM manager.
void AEnemyUnitManager::UpdateHISMPositions()
{
	for (auto& AnElem : EnemyTypeMap)
	{
		AHISMManager* HISMManager = nullptr;
		HISMManager = HISMManagers[AnElem.Key];
		
		if (HISMManager != nullptr)
		{
			for (int i = 0; i < AnElem.Value.Num(); i++)
			{
				HISMManager->TransformIM(AnElem.Value[i].UnitID, AnElem.Value[i].Sphere->GetComponentTransform());
			}
		}
	}
}

void AEnemyUnitManager::ForceUnit(uint32 AUnitID, FVector Direction, float AnAmount)
{
	FEnemyUnitData* UnitData = nullptr;
	UnitData = UnitIDMap.Find(AUnitID);
	if (UnitData != nullptr)
	{
		UnitData->Sphere->AddForce(Direction * AnAmount);
	}
}
uint32 AEnemyUnitManager::SpawnEnemyUnit(EEnemy AnEnemy, FTransform AnInitialTransform)
{
	USphereComponent* SphereComponent = CreateSphere(AnInitialTransform);
	FEnemyUnitData EnemyUnit;
	EnemyUnit.UnitType = AnEnemy;
	EnemyUnit.Sphere = SphereComponent;
	EnemyTypeMap[AnEnemy].Add(EnemyUnit);
	++UnitIDCounter;
	EnemyUnit.UnitID = UnitIDCounter;
	UnitIDMap.Add(EnemyUnit.UnitID, EnemyUnit);

	AHISMManager* HISMManager = *HISMManagers.Find(AnEnemy);

	if (HISMManager != nullptr)
	{
		HISMManager->SpawnIM(EnemyUnit.UnitID, AnInitialTransform);
	}
	return EnemyUnit.UnitID;
}

// Called when the game starts or when spawned
void AEnemyUnitManager::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		AHISMManager* WalkingZombieHISM = (AHISMManager*)GetWorld()->SpawnActor<AHISMManager>(AHISMManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		AHISMManager* BigZombieHISM = (AHISMManager*)GetWorld()->SpawnActor<AHISMManager>(AHISMManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		HISMManagers.Add(EEnemy::SlowZombie, WalkingZombieHISM);
		HISMManagers.Add(EEnemy::BigZombie, BigZombieHISM);


		if (!LevelGenerationActor->HasGenerated)
		{
			LevelGenerationActor->OnGenerateWorld.AddUObject(this, &AEnemyUnitManager::TestSpawn);
		}
		else
		{
			TestSpawn();
		}
	}
}


void AEnemyUnitManager::TestSpawn()
{
}