// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructLibrary.h"
#include "Runtime/AIModule/Public/GraphAStar.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/KismetMathLibrary.h"
#include <Engine/NetSerialization.h>
#include "LevelGeneration.generated.h"

DECLARE_MULTICAST_DELEGATE(FWorldGenerated);

struct FEnemyKeyframe
{
	TArray<FVector_NetQuantize> EnemyPositions;
	float Time;
	FEnemyKeyframe()
	{

	}
	FEnemyKeyframe(TArray<FVector_NetQuantize> AnEnemyPositions, float ACurrentTime)
	{
		EnemyPositions = AnEnemyPositions;
		Time = ACurrentTime;
	}
};
struct FASGraph
{
public:
	typedef int FNodeRef;


	TArray<uint8> Nodes;
	int GridSize;

	static UObject* WorldContextObject;
	static UWorld* InWorld;

	//TArray<FASNode> ASNodes;
	
	TArray<int> GetNeighbourList(FNodeRef NodeRef) const
	{
		TArray<int> Neighbors;
		//x value
		int X = ((NodeRef) % GridSize) + 1;
		//y value
		int Y = (NodeRef) / GridSize + 1;

		bool Up = (Y > 1);
		bool Down = (Y < GridSize);
		bool Left = (X > 1);
		bool Right = (X < GridSize);

		if (Up && Left && Nodes[NodeRef - GridSize - 1] == 0)
			Neighbors.Add(NodeRef - GridSize - 1);

		if (Up && Nodes[NodeRef - GridSize] == 0)
			Neighbors.Add(NodeRef - GridSize);

		if (Up && Right && Nodes[NodeRef - GridSize + 1] == 0)
			Neighbors.Add(NodeRef - GridSize + 1);


		if (Right && Nodes[NodeRef + 1] == 0)
			Neighbors.Add(NodeRef + 1);


		if (Down && Right && Nodes[NodeRef + GridSize + 1] == 0)
			Neighbors.Add(NodeRef + GridSize + 1);


		if (Down && Nodes[NodeRef + GridSize] == 0)
			Neighbors.Add(NodeRef + GridSize);

		if (Down && Left && Nodes[NodeRef + GridSize - 1] == 0)
			Neighbors.Add(NodeRef + GridSize - 1);

		if (Left && Nodes[NodeRef - 1] == 0)
			Neighbors.Add(NodeRef - 1);

		return Neighbors;
	}


	int32 GetNeighbourCount(FNodeRef ANode) const
	{
		return (int32)GetNeighbourList(ANode).Num();
	}

	FNodeRef GetNeighbour(const FNodeRef NodeRef, const int32 NeighbourIndex) const
	{
		return (int32)GetNeighbourList(NodeRef)[NeighbourIndex];
	}

	bool IsValidRef(FNodeRef NodeRef) const
	{
		return true;
	}

};

class FQueryFilter {
public:

	int MapSize = 256;
	TArray<uint8> Maze;

	float GetHeuristicScale() const
	{
		return 1;
	}
	float GetHeuristicCost(const FASGraph::FNodeRef StartNodeRef, const FASGraph::FNodeRef EndNodeRef) const
	{

		int StartX = (StartNodeRef + 1) % MapSize;
		int StartY = (StartNodeRef + 1) / MapSize + 1;

		int EndX = (EndNodeRef + 1) % MapSize;
		int EndY = (EndNodeRef + 1) / MapSize + 1;




		//dx = abs(node.x - goal.x)
		int AbsX = FGenericPlatformMath::Abs(StartX - EndX);
		//dy = abs(node.y - goal.y)
		int AbsY = FGenericPlatformMath::Abs(StartY - EndY);

		//Return the distance with the diagonal moves counting for 1.41
		return (AbsX + AbsY) + (1.41 - 2) * UKismetMathLibrary::Min(AbsX, AbsY);

	}
	float GetTraversalCost(const FASGraph::FNodeRef StartNodeRef, const FASGraph::FNodeRef EndNodeRef) const
	{
		int StartX = (StartNodeRef + 1) % MapSize;
		int StartY = (StartNodeRef + 1) / MapSize + 1;

		int EndX = (EndNodeRef + 1) % MapSize;
		int EndY = (EndNodeRef + 1) / MapSize + 1;




		//dx = abs(node.x - goal.x)
		int AbsX = FGenericPlatformMath::Abs(StartX - EndX);
		//dy = abs(node.y - goal.y)
		int AbsY = FGenericPlatformMath::Abs(StartY - EndY);

		//Return the distance with the diagonal moves counting for 1.41
		return (AbsX + AbsY) + (1.41 - 2) * UKismetMathLibrary::Min(AbsX, AbsY);
	
	}
	bool IsTraversalAllowed(const FASGraph::FNodeRef NodeA, const FASGraph::FNodeRef NodeB) const
	{
		return (Maze[NodeB] != 1 && Maze[NodeA] != 1);
	}

	bool WantsPartialSolution() const
	{
		return false;
	}

};


USTRUCT()
struct FGenerationData
{
	GENERATED_BODY()

	EWorldGridType Type = EWorldGridType::None;
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

	virtual void Tick(float DeltaTime) override;


	void SpawnEnemies();

	void CreateCollisionCubes();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	FVector2D GetVectorFromNeighbors(TArray<uint8> ACollisionGrid, TArray<uint16> HeatMap, int AnIndex);
	TArray<int> GetNeighbourList(TArray<uint8> CollisionGrid, TArray<bool> ACheckedMap, int AnIndex);
	void WaveFrontAlgorithm();

	UFUNCTION(Exec)
	void GenerateWorld();

	int FindValidSeed();
	//Input the start index and then how long along and down you want the grid to be
	TArray<EWorldGridType> GetGridPortion(TArray<EWorldGridType> AGrid, int AFromIndex, int AnXSize, int AYSize);
	TArray<EWorldGridType> GetGridPortionAround(TArray<EWorldGridType> AGrid, int AFromIndex, int ASize);
	int GetGridIndex(FVector APostion);
	int GetGridIndex(int AnX, int AY);

	float AStarPathLength(TArray<uint8> AMaze, int AMazeSize, int AStart, int AnEnd);
	void GenerateGrids(int ASeed);

	bool TestGrids();

	void GenerateCollisionGrid();

	void MakeMiniMapTexture();

	UFUNCTION()
	void OnRep_SetSeed();

	UFUNCTION()
	void OnRep_UpdatePositions();

	void UpdateClientEnemies(float DeltaTime);
	void UpdateEnemies(float DeltaTime);

	void SpawnMeshes(FGenerationData AGenerationData, TArray<EWorldGridType> AFromGrid, TArray<float> AFromElevation);
	void AddGridToGrid(TArray<float> AFromGrid, TArray<EWorldGridType>& AToGrid, TArray<float>& AToElevation, float ACutOff, bool AMoreThan, EWorldGridType AType);
	TArray<float> CreateSimplexGrid(int ASeed, int ASize, float AFrequency);
	void Setup();

public:	

	TArray<EWorldGridType> ResourcesGrid;
	TArray<EWorldGridType> GroundGrid;

	TArray<EWorldGridType> BuildingGrid;
	TArray<FVector2D> VectorMap;

	TArray<float> ResourcesElevation;
	TArray<float> GroundElevation;

	FWorldGenerated OnGenerateWorld;

	int WorldGridSize = 256;

	int GridPositionSize = 50;

	TArray<uint8> CollisionGrid;

	TArray<class USphereComponent*> SphereComponents;

	UPROPERTY(EditAnywhere)
	float Frequency = 1;

	UPROPERTY(EditAnywhere)
	float AboveFloat = 0.5;

	/*float TreeFrequency;
	float TreeCutOff;
	float RockFrequency;
	float RockCutOff;*/

	class UStaticMesh* TreeMesh = nullptr;

	//UPROPERTY(replicated, VisibleAnywhere)
	UPROPERTY(replicated, VisibleAnywhere)
	class USceneComponent* SceneComponent = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* TreeHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* FortGolemHISM = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* RockHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* MudHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* GrassHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* CoalHISMC = nullptr;
	class UHierarchicalInstancedStaticMeshComponent* IronHISMC = nullptr;

	class UHierarchicalInstancedStaticMeshComponent* WaterHISMC = nullptr;


	UPROPERTY(ReplicatedUsing = OnRep_SetSeed)
	int Seed = 0;

	bool HasGenerated = false;

	UPROPERTY(VisibleAnywhere)
	class USceneCaptureComponent2D* CaptureComponent = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UTextureRenderTarget2D* RenderTarget = nullptr;

	UPROPERTY(VisibleAnywhere)
		class UTexture2D* MiniMapTexture = nullptr;

	TMap<EWorldGridType, FGenerationData> GenerationData;

	UPROPERTY(ReplicatedUsing = OnRep_UpdatePositions)
	TArray<FVector_NetQuantize> EnemyPositions;
	FEnemyKeyframe CurrentEnemyFrame;
};
