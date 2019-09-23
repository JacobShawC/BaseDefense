// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HISMManager.h"
#include "StructLibrary.h"
#include "UnitManager.generated.h"


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
	uint32 UnitIDCounter = 0;

	class USphereComponent* CreateSphere(FTransform AnInitialTransform);


	void Tick(float DeltaTime);





protected:
	class ALevelGeneration* LevelGenerationActor = nullptr;
public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	

};
