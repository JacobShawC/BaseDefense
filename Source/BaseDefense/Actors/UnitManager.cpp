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
	
}
// Called when the game starts or when spawned
void AUnitManager::BeginPlay()
{
	Super::BeginPlay();
	ABDGameState* GameState = Cast<ABDGameState>(GetWorld()->GetGameState());

	LevelGenerationActor = GameState->LevelGenerationActor;
}






USphereComponent* AUnitManager::CreateSphere(FTransform AnInitialTransform)
{
	//First we create 
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
	return SphereComponent;
}



void AUnitManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

