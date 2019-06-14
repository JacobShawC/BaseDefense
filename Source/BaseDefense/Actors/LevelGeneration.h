// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructLibrary.h"
#include "LevelGeneration.generated.h"


USTRUCT()
struct FGenerationData
{
	GENERATED_BODY()

	WorldGridType Type = WorldGridType::None;
	class UHierarchicalInstancedStaticMeshComponent* HISM = nullptr;
	float CutOff = 1;
	float Frequency = 1;
	FVector BaseModelSize = FVector(1);
	FVector MaxModelSize = FVector(1);
	float RandHeightVariance = 0;
	float RandWidthVariance = 0;
	float RandXYVariance = 0;
	float RandZVariance = 0;
	bool RotateRandomly = false;
	float ZHeight = 0;
	bool InvertPlacement = false;
	bool ScaleWithHeigh = false;
};


UCLASS()
class BASEDEFENSE_API ALevelGeneration : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGeneration();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Exec)
	void GenerateWorld();



	void SpawnMeshes(FGenerationData AGenerationData, TArray<WorldGridType> AFromGrid, TArray<float> AFromElevation);
	void AddGridToGrid(TArray<float> AFromGrid, TArray<WorldGridType>& AToGrid, TArray<float>& AToElevation, float ACutOff, bool AMoreThan, WorldGridType AType);
	TArray<float> CreateSimplexGrid(int ASeed, int ASize, float AFrequency);
	void Setup();

public:	

	TArray<WorldGridType> TerrainGrid;
	TArray<WorldGridType> GroundGrid;

	TArray<float> TerrainElevation;
	TArray<float> GroundElevation;


	int WorldGridSize = 256;

	UPROPERTY(EditAnywhere)
	float Frequency = 1;

	UPROPERTY(EditAnywhere)
	float AboveFloat = 0.5;

	/*float TreeFrequency;
	float TreeCutOff;
	float RockFrequency;
	float RockCutOff;*/

	class UStaticMesh* TreeMesh = nullptr;
	class USceneComponent* SceneComponent = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* TreeHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* RockHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* MudHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* GrassHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* CoalHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* IronHISMC = nullptr;

	bool HasSetUp = false;
};
