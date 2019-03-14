// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructLibrary.h"
#include "SpawnBox.generated.h"

UCLASS()
class BASEDEFENSE_API ASpawnBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnBox();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	

	UFUNCTION()
	void SpawnEnemy(EEnemy AEnemy);
	
	//void SpawnEnemy(EEnemy AnEnemyType);

public:
	UPROPERTY(EditAnywhere)
	int ID = 0;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* SpawnBoxComponent = nullptr;
};
