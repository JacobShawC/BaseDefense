// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HISMManager.h"
#include "StructLibrary.h"
#include "UnitManager.generated.h"


UENUM()
enum class ETest : uint8
{
	None,
	WalkingZombie,
	BigZombie,
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

	void SpawnActor(FUnitData AUnit);

	void TestSpawn();
	TMap<class USphereComponent*, FUnitData> UnitMap;

protected:
	class ALevelGeneration* LevelGenerationActor = nullptr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY()
	TMap<ETest, AHISMManager*> HISMManagers;

};
