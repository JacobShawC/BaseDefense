// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructLibrary.h"
#include "BaseLevel.generated.h"

UCLASS()
class BASEDEFENSE_API ABaseLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void RunLevel();
	void SpawnWave(EEnemy AnEnemyType, int ABoxNumber, float AFrequency, float AStartSeconds, float AEndSeconds);
	void LogEvent(ELevelEvent AEventType, EEnemy AnEnemyType, int ABoxNumber, float AFrequency, float AStartSeconds, float AEndSeconds);

public:
	float EventTimeCounter = 0;
	TArray<class ASpawnBox*> SpawnBoxes;

	TArray<FLevelEvent> Events;
};
