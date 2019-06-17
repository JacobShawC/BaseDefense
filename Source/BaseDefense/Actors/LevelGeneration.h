// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructLibrary.h"
#include "Runtime/AIModule/Public/GraphAStar.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "LevelGeneration.generated.h"

//struct FASNode 
//{
//public:
//
//	int Index;
//	bool Block;
//
//	TArray<int> NeighborList;
//
//	FASNode();
//	FASNode(int AnIndex, bool ABlock)
//	{
//		Index = AnIndex;
//		Block = ABlock;
//	}
//	FASNode(TArray<int> ANeighborList, int AnIndex, bool ABlock)
//	{
//		Index = AnIndex;
//		Block = ABlock;
//		NeighborList = ANeighborList;
//	}
//
//
//	bool operator == (const FASNode& Other) const
//	{
//		return (Index == Other.Index);
//	}
//};

struct FASGraph
{
public:
	typedef int FNodeRef;


	TArray<uint8> Nodes;
	int GridSize;

	//TArray<FASNode> ASNodes;
	
	TArray<int> GetNeighbourList(FNodeRef NodeRef)  const
	{
		TArray<int> Neighbors;

		int Quotiant = (NodeRef + 1) / GridSize;
		int Remainder = (NodeRef + 1) % GridSize;

		//Check can go up
		if (Quotiant > 0 && Nodes[NodeRef - GridSize - 1] == 0)
		{
			Neighbors.Add(NodeRef - GridSize);
		}

		//check can go left
		if (Remainder > 1 && Nodes[NodeRef - 2] == 0)
		{
			Neighbors.Add(NodeRef - 1);
		}

		//check can go right
		if (Remainder < GridSize && Nodes[NodeRef] == 0)
		{
			Neighbors.Add(NodeRef + 1);
		}

		//check can go Down
		if (Quotiant < GridSize && Nodes[NodeRef + GridSize - 1] == 0)
		{
			Neighbors.Add(NodeRef + GridSize);
		}
	}



	int32 GetNeighbourCount(FNodeRef ANode)
	{
		return GetNeighbourList(ANode).Num();
	}

	FNodeRef GetNeighbour(const FNodeRef NodeRef, const int32 NeighbourIndex) const
	{
		return GetNeighbourList(NodeRef)[NeighbourIndex];
	}

};





USTRUCT()
struct FANode
{
	GENERATED_BODY()

		int Index;
	int ParentIndex;
	int X;
	int Y;

	int G = 0;
	int H = 0;
	int F = 0;

	FANode();
	FANode(int AnIndex, int AParentIndex, int AnX, int AY)
	{
		Index = AnIndex;
		ParentIndex = AParentIndex;

		X = AnX;
		Y = AY;
	}

	FORCEINLINE bool operator==(const FANode& Other) const
	{
		return (X == Other.X && Y == Other.Y);
	}
};


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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(Exec)
	void GenerateWorld(int ASeed);

	int FindValidSeed();


	TArray<FANode*> AStar(TArray<uint8> AMaze, int AMazeSize, int AStart, int AnEnd);
	void GenerateGrids(int ASeed);

	bool TestGrids();

	void GenerateWorldFromGrids(int ASeed);

	UFUNCTION()
	void OnRep_SetSeed();

	void SpawnMeshes(FGenerationData AGenerationData, TArray<WorldGridType> AFromGrid, TArray<float> AFromElevation);
	void AddGridToGrid(TArray<float> AFromGrid, TArray<WorldGridType>& AToGrid, TArray<float>& AToElevation, float ACutOff, bool AMoreThan, WorldGridType AType);
	TArray<float> CreateSimplexGrid(int ASeed, int ASize, float AFrequency);
	void Setup();

public:	

	TArray<WorldGridType> TerrainGrid;
	TArray<WorldGridType> GroundGrid;

	TArray<WorldGridType> BuildingGrid;

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

	//UPROPERTY(replicated, VisibleAnywhere)
	UPROPERTY(replicated, VisibleAnywhere)
	class USceneComponent* SceneComponent = nullptr;
	//UPROPERTY(replicated, VisibleAnywhere)
	UPROPERTY(VisibleAnywhere)
	class UHierarchicalInstancedStaticMeshComponent* TreeHISMC = nullptr;
	//UPROPERTY(replicated, VisibleAnywhere)
	UPROPERTY(VisibleAnywhere)
	class UHierarchicalInstancedStaticMeshComponent* RockHISMC = nullptr;
	//UPROPERTY(replicated, VisibleAnywhere)
	UPROPERTY(VisibleAnywhere)
	class UHierarchicalInstancedStaticMeshComponent* MudHISMC = nullptr;
	//UPROPERTY(replicated, VisibleAnywhere)
	UPROPERTY(VisibleAnywhere)
	class UHierarchicalInstancedStaticMeshComponent* GrassHISMC = nullptr;
	//UPROPERTY(replicated, VisibleAnywhere)
	UPROPERTY(VisibleAnywhere)
	class UHierarchicalInstancedStaticMeshComponent* CoalHISMC = nullptr;
	//UPROPERTY(replicated, VisibleAnywhere)
	UPROPERTY(VisibleAnywhere)
	class UHierarchicalInstancedStaticMeshComponent* IronHISMC = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_SetSeed)
	int Seed = 0;

	bool HasGenerated = false;



	TMap<WorldGridType, FGenerationData> GenerationData;
};
