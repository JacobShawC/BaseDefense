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
	UPROPERTY()
	class UHierarchicalInstancedStaticMeshComponent* MeshPool;


	void FinishUpdates();

protected:
	uint32 ActorIDCount = 0;
	TArray<uint32> IDBuffer;

public:
	TMap<uint32, FIMInstance> IDToInstanceMapping;

	void DestroyIM(uint32 AnActorID);

	uint32 SpawnIM(FTransform AnInitialTransform);
	void TransformIM(uint32 AnActorID, FTransform ATransform);
};
