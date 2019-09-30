// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructLibrary.h"
#include "HISMManager.generated.h"



UCLASS()
class BASEDEFENSE_API AHISMManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHISMManager();

protected:
	// Called when the game starts or when spawned
	void FinishUpdates();

protected:
	
	TArray<uint32> IDBuffer;



public:

	void SetStaticMesh(class UStaticMesh* AMesh);
	TMap<uint32, FIMInstance> IDToInstanceMapping;

	void DestroyIM(uint32 AUnitID);

	void SpawnIM(uint32 AUnitID, FTransform AnInitialTransform);
	void TransformIM(uint32 AUnitID, FTransform ATransform);

	UPROPERTY()
	class UHierarchicalInstancedStaticMeshComponent* MeshPool;
};
