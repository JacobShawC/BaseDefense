// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HISMManager.h"
#include "StructLibrary.h"
#include "UnitManager.generated.h"


<<<<<<< HEAD
USTRUCT()
struct FUnitData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	uint32 UnitID;

	UPROPERTY()
	class USphereComponent* Sphere = nullptr;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	float MoveSpeed;

	UPROPERTY()
	float TurnSpeed;

	UPROPERTY()
	float Size;

	UPROPERTY()
	float AttackDamage;

	UPROPERTY()
	float AttackSpeed;

	UPROPERTY()
	float AttackRange;

};
=======
>>>>>>> 9027e835f44aee79149949402f10503ec882f74b

UCLASS()
class BASEDEFENSE_API AUnitManager : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AUnitManager();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
<<<<<<< HEAD
	uint32 UnitIDCounter = 0;

	class USphereComponent* CreateSphere(FTransform AnInitialTransform);
=======
	uint32 UnitIDCount = 0;
	void SpawnUnit(EUnit AUnit, FTransform AnInitialTransform);
	
	void ForceUnit(uint32 AUnitID, FVector Direction, float AnAmount);

	void SetupAllHISMS();

	void SetupHISM(FUnitData AUnitData);

	void PerformActions();
	void PathTowardsPosition(FUnitInstance AnInstance);

	void TestSpawn();
>>>>>>> 9027e835f44aee79149949402f10503ec882f74b


	void Tick(float DeltaTime);

<<<<<<< HEAD

=======
	TMap<uint32, FUnitInstance> UnitIDMap;

	TMap<EUnit, TArray<FUnitInstance>> UnitTypeMap;
	TMap<EUnit, TArray<FUnitData>> UnitDataMap;
	TMap<EUnit, FUnitData> UnitDataMap;
>>>>>>> 9027e835f44aee79149949402f10503ec882f74b



protected:
	class ALevelGeneration* LevelGenerationActor = nullptr;
public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

<<<<<<< HEAD
	
=======
	UPROPERTY()
	TMap<EUnit, AHISMManager*> HISMManagers;
>>>>>>> 9027e835f44aee79149949402f10503ec882f74b

};
