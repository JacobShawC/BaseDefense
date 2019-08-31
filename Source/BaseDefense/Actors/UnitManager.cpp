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

void AUnitManager::SpawnEnemyUnit(EEnemy AUnit, FTransform AnInitialTransform)
{

	//USphereComponent* SphereComponent = NewObject<USphereComponent>(this);
	////SphereComponent->SetConstraintMode(EDOFMode::XYPlane);
	//SphereComponent->GetBodyInstance()->bLockZTranslation = true;
	//SphereComponent->GetBodyInstance()->SetEnableGravity(false);
	//SphereComponent->GetBodyInstance()->bLockXRotation = true;
	//SphereComponent->GetBodyInstance()->bLockYRotation = true;
	//SphereComponent->GetBodyInstance()->bLockZRotation = true;
	//SphereComponent->GetBodyInstance()->SetMaxDepenetrationVelocity(1);
	//SphereComponent->GetBodyInstance()->SetMassScale(0.1);
	//SphereComponent->GetBodyInstance()->SetUseCCD(false);


	//SphereComponent->SetLinearDamping(0.5f);

	//SphereComponent->SetWorldScale3D(FVector(0.7f));
	//UPhysicalMaterial* PhysMat = NewObject<UPhysicalMaterial>(this);
	//PhysMat->Friction = 0;
	//SphereComponent->SetPhysMaterialOverride(PhysMat);
	//SphereComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//SphereComponent->SetCollisionResponseToAllChannels(ECR_Block);
	//SphereComponent->SetSimulatePhysics(true);
	//SphereComponent->SetWorldLocation(FVector(-X, Y, 15));
	////SphereComponent->SetHiddenInGame(false);
	//SphereComponent->RegisterComponent();
	//SphereComponents.Add(SphereComponent);

	//FTransform Trans = FTransform(FRotator(0), FVector(0));
}

void AUnitManager::TestSpawn()
{
	//AHISMManager* WalkingZombieHISM = (AHISMManager*)GetWorld()->SpawnActor<AHISMManager>(AHISMManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	//AHISMManager* BigZombieHISM = (AHISMManager*)GetWorld()->SpawnActor<AHISMManager>(AHISMManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	//HISMManagers.Add(EUnit::WalkingZombie, WalkingZombieHISM);
	//HISMManagers.Add(EUnit::BigZombie, BigZombieHISM);

	//int ZombieNumber = 2000;
	//if (LevelGenerationActor != nullptr)
	//{
	//	int Number = 0;
	//	for (int i = 0; i < LevelGenerationActor->CollisionGrid.Num(); i++)
	//	{
	//		if (LevelGenerationActor->CollisionGrid[i] != 1)
	//		{
	//			//int X = (i % WorldGridSize) * GridSize;
	//			////y value
	//			//int Y = (i / WorldGridSize) * GridSize;

	//			int Remainder = i % LevelGenerationActor->WorldGridSize;
	//			int Quotient = i / LevelGenerationActor->WorldGridSize;
	//			int X = Remainder * LevelGenerationActor->GridSize + LevelGenerationActor->GridSize / 2;
	//			int Y = Quotient * LevelGenerationActor->GridSize + LevelGenerationActor->GridSize / 2;

	//			USphereComponent* SphereComponent = NewObject<USphereComponent>(this);
	//			//SphereComponent->SetConstraintMode(EDOFMode::XYPlane);
	//			SphereComponent->GetBodyInstance()->bLockZTranslation = true;
	//			SphereComponent->GetBodyInstance()->SetEnableGravity(false);
	//			SphereComponent->GetBodyInstance()->bLockXRotation = true;
	//			SphereComponent->GetBodyInstance()->bLockYRotation = true;
	//			SphereComponent->GetBodyInstance()->bLockZRotation = true;
	//			SphereComponent->GetBodyInstance()->SetMaxDepenetrationVelocity(1);
	//			SphereComponent->GetBodyInstance()->SetMassScale(0.1);
	//			SphereComponent->GetBodyInstance()->SetUseCCD(false);


	//			SphereComponent->SetLinearDamping(0.5f);

	//			SphereComponent->SetWorldScale3D(FVector(0.7f));
	//			UPhysicalMaterial* PhysMat = NewObject<UPhysicalMaterial>(this);
	//			PhysMat->Friction = 0;
	//			SphereComponent->SetPhysMaterialOverride(PhysMat);
	//			SphereComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//			SphereComponent->SetCollisionResponseToAllChannels(ECR_Block);
	//			SphereComponent->SetSimulatePhysics(true);
	//			SphereComponent->SetWorldLocation(FVector(-X, Y, 15));
	//			//SphereComponent->SetHiddenInGame(false);
	//			SphereComponent->RegisterComponent();
	//			SphereComponents.Add(SphereComponent);

	//			FTransform Trans = FTransform(FRotator(0), FVector(0));



	//			FortGolemHISM->AddInstance(FTransform(Trans));
	//			if (EnemyPositions.Num() - 1 > i)
	//			{
	//				//EnemyPositions[i] = FVector_NetQuantize(Trans.GetLocation());
	//			}
	//			else
	//			{
	//				//EnemyPositions.Insert(FVector_NetQuantize(Trans.GetLocation()), i);
	//				EnemyPositions.AddUninitialized();
	//			}

	//			Number++;
	//		}
	//		if (Number == 2000) break;
	//	}
	//}
}

// Called every frame
//void AUnitManager::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

