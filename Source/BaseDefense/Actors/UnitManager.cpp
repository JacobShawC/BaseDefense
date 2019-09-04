// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitManager.h"
#include "BDGameState.h"
#include "LevelGeneration.h"
#include <Engine/World.h>
#include "HISMManager.h"
#include <Components/SphereComponent.h>
#include <PhysicalMaterials/PhysicalMaterial.h>

// Sets default values
AUnitManager::AUnitManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FEnemyUnitData SlowZombieData;
	//SlowZombieData.
	//EnemyDataMap

}

// Called when the game starts or when spawned
void AUnitManager::BeginPlay()
{
	Super::BeginPlay();
	int Test = 8;
	if (Role == ROLE_Authority)
	{
		AHISMManager* WalkingZombieHISM = (AHISMManager*)GetWorld()->SpawnActor<AHISMManager>(AHISMManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		AHISMManager* BigZombieHISM = (AHISMManager*)GetWorld()->SpawnActor<AHISMManager>(AHISMManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		HISMManagers.Add(EEnemy::SlowZombie, WalkingZombieHISM);
		HISMManagers.Add(EEnemy::BigZombie, BigZombieHISM);

		ABDGameState* GameState = Cast<ABDGameState>(GetWorld()->GetGameState());
		LevelGenerationActor = GameState->LevelGenerationActor;

		if (!LevelGenerationActor->HasGenerated)
		{
			LevelGenerationActor->OnGenerateWorld.AddUObject(this, &AUnitManager::TestSpawn);
		}
		else
		{
			TestSpawn();
		}
	}
}

void AUnitManager::ForceUnit(uint32 AUnitID, FVector Direction, float AnAmount)
{
	FEnemyUnitData* UnitData = nullptr;
	UnitData = UnitIDMap.Find(AUnitID);
	if (UnitData != nullptr)
	{
		UnitData->Sphere->AddForce(Direction * AnAmount);
	}
}

void AUnitManager::SpawnEnemyUnit(EEnemy AUnit, FTransform AnInitialTransform)
{
	USphereComponent* SphereComponent = NewObject<USphereComponent>(this);
	//SphereComponent->SetConstraintMode(EDOFMode::XYPlane);
	SphereComponent->GetBodyInstance()->bLockZTranslation = true;
	SphereComponent->GetBodyInstance()->SetEnableGravity(false);
	SphereComponent->GetBodyInstance()->bLockXRotation = true;
	SphereComponent->GetBodyInstance()->bLockYRotation = true;
	SphereComponent->GetBodyInstance()->bLockZRotation = true;
	SphereComponent->GetBodyInstance()->SetMaxDepenetrationVelocity(1);
	SphereComponent->GetBodyInstance()->SetMassScale(0.1);
	SphereComponent->GetBodyInstance()->SetUseCCD(false);

	SphereComponent->SetLinearDamping(0.5f);

	SphereComponent->SetWorldScale3D(FVector(0.7f));
	UPhysicalMaterial* PhysMat = NewObject<UPhysicalMaterial>(this);
	PhysMat->Friction = 0;
	SphereComponent->SetPhysMaterialOverride(PhysMat);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetWorldTransform(AnInitialTransform);
	SphereComponent->SetHiddenInGame(true);
	SphereComponent->RegisterComponent();
	FEnemyUnitData EnemyUnit;
	EnemyUnit.UnitType = AUnit;
	EnemyUnit.Sphere = SphereComponent;
	EnemyTypeMap[AUnit].Add(EnemyUnit);
	++UnitIDCount;
	EnemyUnit.UnitID = UnitIDCount;
	UnitIDMap.Add(EnemyUnit.UnitID, EnemyUnit);

	AHISMManager* HISMManager = *HISMManagers.Find(AUnit);

	if (HISMManager != nullptr)
	{
		HISMManager->SpawnIM(EnemyUnit.UnitID, AnInitialTransform);
	}
}

void AUnitManager::TestSpawn()
{
}

void AUnitManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//We go though every unit's sphere and update the instances position via their HISM manager.
void AUnitManager::UpdateHISMPositions()
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
				int test = 888;
			}
		}
		
	}
}

