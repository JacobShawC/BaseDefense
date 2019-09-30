// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitManager.h"
#include "BDGameState.h"
#include "LevelGeneration.h"
#include "BDGameInstance.h"
#include <Engine/World.h>
#include "HISMManager.h"
#include <Components/SphereComponent.h>
#include <PhysicalMaterials/PhysicalMaterial.h>
#include "BDSphereComponent.h"

// Sets default values
AUnitManager::AUnitManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentFrame.OnItemAddedOrChanged.AddUObject(this, &AUnitManager::OnItemChangedOrAdded);
	CurrentFrame.OnItemRemoved.AddUObject(this, &AUnitManager::OnItemRemoved);
}

// Called when the game starts or when spawned

//When the game starts we set up all of the HISM managers.
void AUnitManager::BeginPlay()
{
	Super::BeginPlay();

	UBDGameInstance* GameInstance = nullptr;
	GameInstance = GetWorld()->GetGameInstance<UBDGameInstance>();
	if (GameInstance != nullptr)
	{
		UnitDataMap = GameInstance->Units;
	}
	SetupAllHISMS();
	if (Role == ROLE_Authority)
	{
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
//applies a force in a direction to a unit in a specific unit, takes the unit id.
void AUnitManager::ForceUnit(uint32 AUnitID, FVector Direction, float AnAmount)
{
	FUnitInstance* UnitInstance = nullptr;
	UnitInstance = UnitIDMap.Find(AUnitID);
	if (UnitInstance != nullptr)
	{
		UnitInstance->CollisionSphere->AddForce(Direction * AnAmount);

	}
}
//Goes through the unit data map on the gameinstance and spawns a hism manager for each one.
void AUnitManager::SetupAllHISMS()
{
	for (auto& AUnitData : UnitDataMap)
	{
		SetupHISM(AUnitData.Value);
	}
}
//Spawns a HISM Manager and adds it to the Manager map.
void AUnitManager::SetupHISM(FUnitData AUnitData)
{
	AHISMManager* HISM = (AHISMManager*)GetWorld()->SpawnActor<AHISMManager>(AHISMManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	HISMManagers.Add(AUnitData.Type, HISM);
}

void AUnitManager::SpawnUnit(EGameUnit AUnit, FTransform AnInitialTransform)
{
	FUnitData UnitData = UnitDataMap[AUnit];


	UBDSphereComponent* CollisionSphere = NewObject<UBDSphereComponent>(this);
	//SphereComponent->SetConstraintMode(EDOFMode::XYPlane);
	CollisionSphere->GetBodyInstance()->bLockZTranslation = true;
	CollisionSphere->GetBodyInstance()->SetEnableGravity(false);
	CollisionSphere->GetBodyInstance()->bLockXRotation = true;
	CollisionSphere->GetBodyInstance()->bLockYRotation = true;
	CollisionSphere->GetBodyInstance()->bLockZRotation = true;
	CollisionSphere->GetBodyInstance()->SetMaxDepenetrationVelocity(1);
	CollisionSphere->GetBodyInstance()->SetMassScale(0.1);
	CollisionSphere->GetBodyInstance()->SetUseCCD(false);
	CollisionSphere->SetLinearDamping(0.5f);
	CollisionSphere->SetGenerateOverlapEvents(false);

	CollisionSphere->SetWorldScale3D(FVector(UnitData.Size));
	UPhysicalMaterial* PhysMat = NewObject<UPhysicalMaterial>(this);
	PhysMat->Friction = 0;
	CollisionSphere->SetPhysMaterialOverride(PhysMat);
	CollisionSphere->SetSimulatePhysics(true);
	CollisionSphere->SetWorldTransform(AnInitialTransform);
	CollisionSphere->SetHiddenInGame(true);
	CollisionSphere->RegisterComponent();

	//Add the sphere component for the attack range.

	USphereComponent* RangeSphere = nullptr;

	if (UnitData.AttackRange != 0)
	{
		RangeSphere = NewObject<USphereComponent>(this);
		RangeSphere->SetWorldScale3D(FVector(UnitData.AttackRange));
		RangeSphere->SetGenerateOverlapEvents(true);
		RangeSphere->AttachToComponent(CollisionSphere, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		RangeSphere->SetSimulatePhysics(false);
		FScriptDelegate OverlapDelegate;
		OverlapDelegate.BindUFunction(this, "OnRangeOverlap");
		RangeSphere->OnComponentBeginOverlap.Add(OverlapDelegate);
	}

	 

	if (UnitData.Team == ETeam::Enemy)
	{
		//Set collision to enemy type.
		CollisionSphere->SetCollisionProfileName("EnemyUnit");

		//Set range to react to friendly type.
		if (RangeSphere != nullptr)
		{
			RangeSphere->SetCollisionProfileName("EnemyRange");
		}

	}
	else if (UnitData.Team == ETeam::Friendly)
	{
		//Set collision to enemy type.
		CollisionSphere->SetCollisionProfileName("FriendlyUnit");

		//Set range to react to friendly type.
		if (RangeSphere != nullptr)
		{
			RangeSphere->SetCollisionProfileName("FriendlyRange");
		}
	}

	++UnitIDCount;
	FUnitInstance Unit;
	//If is an enemy unit start off pathing to center of map.
	if (UnitData.Team == ETeam::Enemy)
	{
		Unit.PathingDestination = LevelGenerationActor->WorldGridSize / 2;
	}
	Unit.Type = AUnit;
	Unit.PushForce = UnitData.PushForce;
	Unit.Size = UnitData.PushForce;
	Unit.CollisionSphere = CollisionSphere;
	UnitTypeMap[AUnit].Add(Unit);
	//Add the ID to the Unit and the collisionsphere
	CollisionSphere->ID = UnitIDCount;
	Unit.ID = UnitIDCount;

	UnitIDMap.Add(Unit.ID, Unit);

	AHISMManager* HISMManager = *HISMManagers.Find(AUnit);

	if (HISMManager != nullptr)
	{
		HISMManager->SpawnIM(Unit.ID, AnInitialTransform);
	}
}

void AUnitManager::TestSpawn()
{
}
//Apply pathfinding forces, update HISM positions and update multiplayer data.
void AUnitManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PerformActions();
	UpdateHISMPositions();
}
//ai ideas.
//rules
//actions
//triggers
//data driven
//could be priority list:
//1. walk to location
//2. attack team ally

//Second idea: Units do things based on set states/data such as target location. These states are changed by outside forces and not by the unit manager. IE if tower attacks zombie, tower sets zombie's target to itself. If tower dies. tower sets zombies target to default. This means the unit manager does not need to constantly check what each zombie should be doing.

void AUnitManager::PerformActions()
{
	for (auto& AnElem : UnitTypeMap)
	{
		FUnitData* UnitData = nullptr;
		UnitData = UnitDataMap.Find(AnElem.Key);

		if (UnitData != nullptr)
		{
			for (int i = 0; i < AnElem.Value.Num(); i++)
			{
				switch (AnElem.Value[i].CurrentAction)
				{
					case EUnitAction::None: break;
					case EUnitAction::Pathing: PathTowardsPosition(AnElem.Value[i]);
					case EUnitAction::Attacking: break;
				}
			}
		}
	}
}

void AUnitManager::SetActionToDefault(FUnitInstance* AUnitInstance)
{
	if (AUnitInstance != nullptr)
	{
		if (AUnitInstance->PathingDestination != 0)
		{
			AUnitInstance->CurrentAction = EUnitAction::Pathing;
		}
	}
}

void AUnitManager::PathTowardsPosition(FUnitInstance AnInstance)
{
	if (LevelGenerationActor != nullptr)
	{
		FTransform Trans;
		FVector Location = AnInstance.CollisionSphere->GetComponentLocation();
		Location = FVector(Location.X, Location.Y, 0.0f);
		int GridSize = LevelGenerationActor->GridPositionSize;
		float WorldGridSize = LevelGenerationActor->WorldGridSize;
		int X = -Location.X / (float)GridSize;
		int Y = Location.Y / (float)GridSize;

		int Index = Y * WorldGridSize + X;
		if (Index < LevelGenerationActor->VectorMap.Num() - 1 && Index > 0)
		{
			float ForceAmount = 80;
			AnInstance.CollisionSphere->AddForce(FVector(-LevelGenerationActor->VectorMap[Index].X * ForceAmount, LevelGenerationActor->VectorMap[Index].Y * ForceAmount, 0));
		}
	}
}

//When the current frame gets replicated we add the new or changed frames to the unit frame array.
void AUnitManager::OnItemChangedOrAdded(FUnitFrameItem& AnItem)
{
	TArray<FUnitFrameItem>* UnitFrameArray = &UnitFrames.FindOrAdd(AnItem.UnitID);
	AnItem.CurrentTime = CurrentFrame.CurrentTime;
	UnitFrameArray->Add(AnItem);
}

//When we remove an item from the array we add a frame with the dead attribute set to true.
void AUnitManager::OnItemRemoved(FUnitFrameItem& AnItem)
{
	TArray<FUnitFrameItem>* UnitFrameArray = &UnitFrames.FindOrAdd(AnItem.UnitID);
	AnItem.CurrentTime = CurrentFrame.CurrentTime;
	AnItem.Dead = true;

	UnitFrameArray->Add(AnItem);
}

void AUnitManager::OnRangeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UBDSphereComponent* BDSphere = Cast<UBDSphereComponent>(OverlappedComp))
	{
		//BDSphere->ID;
	}
}

void AUnitManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnitManager, CurrentFrame);
}

void AUnitManager::OnRep_CurrentFrame()
{
	//Frame updated
}

//We go though every unit's CollisionSphere and update the instances position via their HISM manager.
void AUnitManager::UpdateHISMPositions()
{
	for (auto& AnElem : UnitTypeMap)
	{
		AHISMManager* HISMManager = nullptr;
		HISMManager = HISMManagers[AnElem.Key];

		if (HISMManager != nullptr)
		{
			for (int i = 0; i < AnElem.Value.Num(); i++)
			{
				HISMManager->TransformIM(AnElem.Value[i].ID, AnElem.Value[i].CollisionSphere->GetComponentTransform());
			}
		}
		
	}
}

void FUnitFrameItem::PreReplicatedRemove(const struct FUnitFrame& InArraySerializer)
{
	InArraySerializer.OnItemRemoved.Broadcast(*this);
}

void FUnitFrameItem::PostReplicatedAdd(const struct FUnitFrame& InArraySerializer)
{
	InArraySerializer.OnItemAddedOrChanged.Broadcast(*this);

}

void FUnitFrameItem::PostReplicatedChange(const struct FUnitFrame& InArraySerializer)
{
	InArraySerializer.OnItemAddedOrChanged.Broadcast(*this);
}
