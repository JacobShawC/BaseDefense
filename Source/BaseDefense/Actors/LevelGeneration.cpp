// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeneration.h"
#include "BDGameInstance.h"
#include "BDGameState.h"
#include "Engine/World.h"
//#include "C:\Users\Ziggy\Documents\Unreal Projects\BaseDefense\Plugins\SimplexNoise-master\Source\SimplexNoise\Public\SimplexNoiseBPLibrary.h"
//#include "Engine/Texture2D.h"
#include "Public/ImageUtils.h"
#include <ConstructorHelpers.h>
#include <Engine/StaticMesh.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Components/HierarchicalInstancedStaticMeshComponent.h>
#include <Math/Quat.h>
#include "StaticFunctionLibrary.h"
//#include "UObject/ConstructorHelpers.h"

// Sets default values
ALevelGeneration::ALevelGeneration()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	TreeHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("TreeHISMC"));
	RockHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("RockHISMC"));
	MudHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("MudHISMC"));
	GrassHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GrassHISMC"));
	CoalHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("CoalHISMC"));
	IronHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("IronHISMC"));

	TreeHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RockHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MudHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GrassHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree(TEXT("StaticMesh'/Game/PolygonFantasyRivals/Meshes/Props/SM_Prop_Tree_02.SM_Prop_Tree_02'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree(TEXT("StaticMesh'/Game/VertexAnimations/SM_Tree2_AnimVertTest_00.SM_Tree2_AnimVertTest_00'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Rock(TEXT("StaticMesh'/Game/PolygonDungeons/Meshes/Environments/Rocks/SM_Env_Rock_Flat_Large_03.SM_Env_Rock_Flat_Large_03'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mud(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Grass(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube_2.1M_Cube_2'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Coal(TEXT("StaticMesh'/Game/Meshes/Coal.Coal'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Iron(TEXT("StaticMesh'/Game/Meshes/Iron.Iron'"));


	TreeHISMC->SetStaticMesh(Tree.Object);
	RockHISMC->SetStaticMesh(Rock.Object);
	MudHISMC->SetStaticMesh(Mud.Object);
	GrassHISMC->SetStaticMesh(Grass.Object);
	CoalHISMC->SetStaticMesh(Coal.Object);
	IronHISMC->SetStaticMesh(Iron.Object);
	CoalHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IronHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//IronHISMC->collision
	//TreeHISMC->SetCullDistances(500, 501);
	//RockHISMC->SetCullDistances(500, 501);
	//GroundHISMC->SetCullDistances(500, 501);


}

// Called when the game starts or when spawned
void ALevelGeneration::BeginPlay()
{
	Super::BeginPlay();

	FGenerationData CoalData;
	CoalData.Type = WorldGridType::Coal;
	CoalData.HISM = CoalHISMC;
	CoalData.Frequency = 10;
	CoalData.CutOff = 0.9;
	//CoalData.BaseModelSize = FVector(3.75, 3.75, 3.75);
	CoalData.ScaleWithHeigh = false;
	CoalData.BaseModelSize = FVector(0.13, 0.13, 0.13);
	//CoalData.InvertPlacement = true;
	CoalData.RotateRandomly = true;

	FGenerationData IronData;
	IronData.Type = WorldGridType::Iron;
	IronData.HISM = IronHISMC;
	IronData.Frequency = 10;
	IronData.CutOff = 0.9;
	//IronData.BaseModelSize = FVector(3.75, 3.75, 3.75);
	IronData.ScaleWithHeigh = false;
	IronData.BaseModelSize = FVector(0.13, 0.13, 0.13);
	//IronData.InvertPlacement = true;
	IronData.RotateRandomly = true;

	FGenerationData TreeData;
	TreeData.Type = WorldGridType::Tree;
	TreeData.HISM = TreeHISMC;
	TreeData.Frequency = 7;
	TreeData.CutOff = 0.7;
	//TreeData.BaseModelSize = FVector(3.75, 3.75, 3.75);
	TreeData.ScaleWithHeigh = true;
	TreeData.BaseModelSize = FVector(1, 1, 1);
	TreeData.MaxModelSize = FVector(2, 2, 3);
	TreeData.RandHeightVariance = 0.25;
	TreeData.RandWidthVariance = 0.125;
	TreeData.RandXYVariance = 0.5;
	//TreeData.InvertPlacement = true;
	TreeData.RotateRandomly = true;

	FGenerationData RockData;
	RockData.Type = WorldGridType::Rock;
	RockData.HISM = RockHISMC;
	RockData.Frequency = 7;
	RockData.CutOff = 0.65;
	RockData.ScaleWithHeigh = true;

	RockData.BaseModelSize = FVector(0.1, 0.1, 0.5);
	RockData.MaxModelSize = FVector(0.1, 0.1, 0.9);
	RockData.RandHeightVariance = 0.2;
	RockData.RandWidthVariance = 0;
	RockData.RandXYVariance = 0.1;
	RockData.RotateRandomly = true;
	//RockData.InvertPlacement = true;
	RockData.ZHeight = 0;


	FGenerationData MudData;
	MudData.Type = WorldGridType::Mud;
	MudData.HISM = MudHISMC;
	MudData.Frequency = 7;
	MudData.CutOff = -100;
	MudData.ScaleWithHeigh = false;
	MudData.BaseModelSize = FVector(0.5, 0.5, 1);
	MudData.RandHeightVariance = 0;
	MudData.RandWidthVariance = 0;
	MudData.RandXYVariance = 0;
	//MudData.InvertPlacement = true;
	MudData.RotateRandomly = false;
	MudData.ZHeight = -50;

	FGenerationData GrassData;
	GrassData.Type = WorldGridType::Grass;
	GrassData.HISM = GrassHISMC;
	GrassData.Frequency = 7;
	GrassData.CutOff = 0.05;
	GrassData.ScaleWithHeigh = false;
	GrassData.BaseModelSize = FVector(0.5, 0.5, 1);
	GrassData.RandHeightVariance = 0;
	GrassData.RandWidthVariance = 0;
	GrassData.RandXYVariance = 0;
	GrassData.RotateRandomly = false;
	GrassData.ZHeight = -50;


	FGenerationData WaterData;
	WaterData.Type = WorldGridType::Water;
	//WaterData.HISM = GrassHISMC;
	WaterData.Frequency = 7;
	WaterData.CutOff = 0.6;
	WaterData.ScaleWithHeigh = false;
	WaterData.BaseModelSize = FVector(0.5, 0.5, 1);
	WaterData.RandHeightVariance = 0;
	WaterData.RandWidthVariance = 0;
	WaterData.RandXYVariance = 0;
	WaterData.RotateRandomly = false;
	WaterData.ZHeight = -50;

	TArray<float> CoalGrid = CreateSimplexGrid(6789, WorldGridSize, CoalData.Frequency);
	TArray<float> IronGrid = CreateSimplexGrid(4567, WorldGridSize, IronData.Frequency);

	TArray<float> TreeGrid = CreateSimplexGrid(1234, WorldGridSize, TreeData.Frequency);
	TArray<float> RockGrid = CreateSimplexGrid(4321, WorldGridSize, RockData.Frequency);
	TArray<float> WaterGrid = CreateSimplexGrid(4567, WorldGridSize, RockData.Frequency);

	//Grass uses the tree grid

	//Mud doesnt need a grid because its default
	TArray<float> MudGrid;
	MudGrid.AddZeroed(WorldGridSize);

	//Coal
	AddGridToGrid(CoalGrid, TerrainGrid, TerrainElevation, CoalData.CutOff, true, CoalData.Type);

	//Iron
	AddGridToGrid(IronGrid, TerrainGrid, TerrainElevation, IronData.CutOff, true, IronData.Type);

	//Trees
	AddGridToGrid(TreeGrid, TerrainGrid, TerrainElevation, TreeData.CutOff, true, TreeData.Type);

	//Rocks
	AddGridToGrid(RockGrid, TerrainGrid, TerrainElevation, RockData.CutOff, true, RockData.Type);

	//Mud
	AddGridToGrid(TreeGrid, GroundGrid, GroundElevation, MudData.CutOff, true, MudData.Type);

	//Grass
	AddGridToGrid(TreeGrid, GroundGrid, GroundElevation, GrassData.CutOff, true, GrassData.Type);

	//Water
	AddGridToGrid(WaterGrid, GroundGrid, GroundElevation, WaterData.CutOff, true, WaterData.Type);
	AddGridToGrid(WaterGrid, TerrainGrid, GroundElevation, WaterData.CutOff, true, WaterData.Type);


	SpawnMeshes(CoalData, TerrainGrid, TerrainElevation);
	SpawnMeshes(IronData, TerrainGrid, TerrainElevation);

	SpawnMeshes(MudData, GroundGrid, GroundElevation);
	SpawnMeshes(GrassData, GroundGrid, GroundElevation);
	SpawnMeshes(TreeData, TerrainGrid, TerrainElevation);
	SpawnMeshes(RockData, TerrainGrid, TerrainElevation);



}

void ALevelGeneration::GenerateWorld()
{

}
void ALevelGeneration::SpawnMeshes(FGenerationData AGenerationData, TArray<WorldGridType> AFromGrid, TArray<float> AFromElevation)
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnMeshes:  AFromGrid.Num(): %i"), AFromGrid.Num());


	for (int i = 0; i < AFromGrid.Num(); i++)
	{
		bool SpawnAMesh = false;

		if (AGenerationData.InvertPlacement == false)
		{
			if (AFromGrid[i] == AGenerationData.Type)
			{
				SpawnAMesh = true;
			}
		}
		else
		{
			if (AFromGrid[i] != AGenerationData.Type)
			{
				SpawnAMesh = true;
			}
		}

		if (SpawnAMesh == true)
		{
			int Remainder = i % WorldGridSize;
			int Quotient = i / WorldGridSize;
			int Y = Remainder * 50;
			int X = Quotient * 50;

			float RandXY = 0;
			float RandZ = 0;
			if (AGenerationData.RandXYVariance > 0)
			{
				RandXY = FMath::FRandRange(0, AGenerationData.RandXYVariance);
				RandZ = FMath::FRandRange(0, AGenerationData.RandZVariance);
			}
			


			//Work out the height at the current elevation
			
			FVector ElevationModelSize = AGenerationData.BaseModelSize;
			
			if (AGenerationData.ScaleWithHeigh == true)
			{
				float ElevationMultiplier = 0.0f;
				if (1 - AGenerationData.CutOff > 0)
				{
					ElevationMultiplier = (AFromElevation[i] - AGenerationData.CutOff) / (1 - AGenerationData.CutOff);
				}

				ElevationModelSize += (AGenerationData.MaxModelSize - AGenerationData.BaseModelSize) * ElevationMultiplier;

			}

			FTransform Trans = FTransform(FVector(-X + RandXY, Y + RandXY, AGenerationData.ZHeight + RandZ));


			if (AGenerationData.RotateRandomly)
			{
				int RandomRotation = FMath::RandRange(0, 359);

				Trans.SetRotation(FQuat((FRotator(0, RandomRotation, 0))));
			}

			float RandHeightVariance = FMath::RandRange(0.0f, AGenerationData.RandHeightVariance);
			float RandWidthVariance = FMath::RandRange(0.0f, AGenerationData.RandWidthVariance);
			FVector TransScale = FVector(ElevationModelSize + FVector(RandWidthVariance, RandWidthVariance, RandHeightVariance));

			int CoinFlip = FMath::RandRange(0, 1);
			if (CoinFlip == 0)
			{
				TransScale = FVector(-TransScale.X, -TransScale.Y, TransScale.Z);
			}

			Trans.SetScale3D(TransScale);

			AGenerationData.HISM->AddInstance(FTransform(Trans));
		}

	}
}

void ALevelGeneration::AddGridToGrid(TArray<float> AFromGrid, TArray<WorldGridType>& AToGrid, TArray<float>& AToElevation, float ACutOff, bool AMoreThan, WorldGridType AType)
{
	int AboveCounter = 0;
	int BelowCounter = 0;

	if (AToGrid.Num() < AFromGrid.Num())
	{
		AToGrid.AddZeroed(AFromGrid.Num() - AToGrid.Num());
	}

	if (AToElevation.Num() < AFromGrid.Num())
	{
		AToElevation.AddZeroed(AFromGrid.Num() - AToElevation.Num());
	}

	for (int i = 0; i < AFromGrid.Num(); i++)
	{
		if (AMoreThan)
		{
			if (AFromGrid[i] >= ACutOff)
			{
				AToGrid[i] = AType;
				AToElevation[i] = AFromGrid[i];
				AboveCounter++;
			}
			else
			{
				BelowCounter++;
			}
		}
		else
		{
			if (AFromGrid[i] < ACutOff)
			{
				AToGrid[i] = AType;
				AToElevation[i] = AFromGrid[i];
				AboveCounter++;
			}
			else
			{
				BelowCounter++;
			}
		}
	}



	//UE_LOG(LogTemp, Warning, TEXT("GenerateGrid: Number above : %f, %i, below : %i, out of %i, %f, %f, %f, %f, %f"), ACutOff, AboveCounter, BelowCounter, AFromGrid.Num(), AFromGrid[0], AFromGrid[1], AFromGrid[2], AFromGrid[3], AFromGrid[4]);
	//UE_LOG(LogTemp, Warning, TEXT("GenerateGrid: Number above : %f, %i, below : %i, out of %i, %f, %f, %f, %f, %f"), ACutOff, AboveCounter, BelowCounter, AFromGrid.Num(), AFromGrid[256], AFromGrid[257], AFromGrid[258], AFromGrid[259], AFromGrid[260]);




	
}

//
TArray<float> ALevelGeneration::CreateSimplexGrid(int ASeed, int ASize, float AFrequency)
{
	//if (!HasSetUp) return;
	TArray<float> Elevation;

	UStaticFunctionLibrary::setNoiseSeed(ASeed);

	for (int y = 0; y < ASize; y++)
	{
		// 
		for (int x = 0; x < ASize; x++)
		{


			float nx = float(x) / float(ASize);
			float ny = float(y) / float(ASize);
			float Data = UStaticFunctionLibrary::SimplexNoiseInRange2D(nx * AFrequency, ny * AFrequency, 0.0f, -1.0f);
			//Elevation.Add(Data / 2 + 0.5f);
			Elevation.Add(Data);
		}
	}

	return Elevation;
}

void ALevelGeneration::Setup()
{
	ABDGameState* GameState = Cast<ABDGameState>(GetWorld()->GetGameState());
	WorldGridSize = GameState->WorldGridSize;
	

}


