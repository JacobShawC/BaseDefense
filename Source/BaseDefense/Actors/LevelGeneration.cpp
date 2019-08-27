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
#include "GenericPlatform/GenericPlatformMath.h"
#include "SearchGraph.h"
#include "Algo/Reverse.h"
#include "Net/UnrealNetwork.h"
#include <Components/SceneCaptureComponent2D.h>
#include <Engine/TextureRenderTarget2D.h>
#include <Engine/Texture2D.h>
#include <UnrealMathSSE.h>
#include "Kismet/KismetSystemLibrary.h"
#include <Engine/Engine.h>
#include <Components/SphereComponent.h>
#include <Components/PrimitiveComponent.h>
#include "PhysicalMaterials/PhysicalMaterial.h"
#include <Components/BoxComponent.h>
#include <UnrealMathUtility.h>

//#include "UObject/ConstructorHelpers.h"

// Sets default values
ALevelGeneration::ALevelGeneration()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	bReplicates = true;
	bAlwaysRelevant = true;

	PrimaryActorTick.bCanEverTick = true;
	this->SetActorScale3D(FVector(1));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(SceneComponent);


	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("USceneCaptureComponent2D"));
	CaptureComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
	CaptureComponent->OrthoWidth = 25600.0f;
	CaptureComponent->SetWorldRotation(FRotator(-90, 0, 0));
	CaptureComponent->SetWorldLocation(FVector(-12700, 12700, 500));
	CaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	CaptureComponent->bCaptureOnMovement = false;
	CaptureComponent->bCaptureEveryFrame = false;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 2;

	TArray<UHierarchicalInstancedStaticMeshComponent*> HISMs;
	TreeHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("TreeHISMC"));
	HISMs.Add(TreeHISMC);
	RockHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("RockHISMC"));
	HISMs.Add(RockHISMC);
	MudHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("MudHISMC"));
	HISMs.Add(MudHISMC);
	GrassHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GrassHISMC"));
	HISMs.Add(GrassHISMC);
	CoalHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("CoalHISMC"));
	HISMs.Add(CoalHISMC);
	IronHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("IronHISMC"));
	HISMs.Add(IronHISMC);
	WaterHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("WaterHISMC"));
	HISMs.Add(WaterHISMC);
	FortGolemHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("FortGolemHISM"));
	HISMs.Add(FortGolemHISM);

	//GlobalSettings
	for (auto Component : HISMs)
	{
		Component->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Component->SetWorldScale3D(FVector(1));

		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Component->SetMobility(EComponentMobility::Movable);

	}


	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree(TEXT("StaticMesh'/Game/VertexAnimations/SM_Tree2_AnimVertTest_00.SM_Tree2_AnimVertTest_00'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree(TEXT("StaticMesh'/Game/PolygonFantasyRivals/Meshes/Props/SM_Prop_Tree_02.SM_Prop_Tree_02'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Rock(TEXT("StaticMesh'/Game/PolygonDungeons/Meshes/Environments/Rocks/SM_Env_Rock_Flat_Large_03.SM_Env_Rock_Flat_Large_03'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mud(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Grass(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube_2.1M_Cube_2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Water(TEXT("StaticMesh'/Game/Geometry/Meshes/WaterCube.WaterCube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FortGolem(TEXT("StaticMesh'/Game/VertexAnimations/SK_BR_Character_FortGolem_2_Test9.SK_BR_Character_FortGolem_2_Test9'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TestCube(TEXT("StaticMesh'/Game/Geometry/Meshes/TestCube.TestCube'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Coal(TEXT("StaticMesh'/Game/Meshes/Coal.Coal'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Iron(TEXT("StaticMesh'/Game/Meshes/Iron.Iron'"));

	GridSize = 100;


	TreeHISMC->SetStaticMesh(Tree.Object);
	RockHISMC->SetStaticMesh(Rock.Object);
	MudHISMC->SetStaticMesh(Mud.Object);
	GrassHISMC->SetStaticMesh(Grass.Object);
	CoalHISMC->SetStaticMesh(Coal.Object);
	IronHISMC->SetStaticMesh(Iron.Object);
	FortGolemHISM->SetStaticMesh(FortGolem.Object);
	//FortGolemHISM->SetStaticMesh(TestCube.Object);
	WaterHISMC->SetStaticMesh(Water.Object);

	/*CoalHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IronHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TreeHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RockHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MudHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GrassHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/

	//IronHISMC->collision
	int Culling = 10000;
	/*TreeHISMC->SetMobility(EComponentMobility::Static);
	RockHISMC->SetMobility(EComponentMobility::Static);
	MudHISMC->SetMobility(EComponentMobility::Static);
	GrassHISMC->SetMobility(EComponentMobility::Static);
	CoalHISMC->SetMobility(EComponentMobility::Static);
	IronHISMC->SetMobility(EComponentMobility::Static);*/
	/*TreeHISMC->InstanceStartCullDistance = Culling;
	RockHISMC->InstanceStartCullDistance = Culling;
	MudHISMC->InstanceStartCullDistance = Culling;
	GrassHISMC->InstanceStartCullDistance = Culling;
	CoalHISMC->InstanceStartCullDistance = Culling;
	IronHISMC->InstanceStartCullDistance = Culling;

	TreeHISMC->InstanceEndCullDistance = Culling;
	RockHISMC->InstanceEndCullDistance = Culling;
	MudHISMC->InstanceEndCullDistance = Culling;
	GrassHISMC->InstanceEndCullDistance = Culling;
	CoalHISMC->InstanceEndCullDistance = Culling;
	IronHISMC->InstanceEndCullDistance = Culling;*/
	 
	/*TreeHISMC->SetCullDistance(Culling);
	RockHISMC->SetCullDistance(Culling);
	MudHISMC->SetCullDistance(Culling);
	GrassHISMC->SetCullDistance(Culling);
	CoalHISMC->SetCullDistance(Culling);
	IronHISMC->SetCullDistance(Culling);*/
	//MudHISMC->InstanceEndCullDistance = 100;
	//MudHISMC->bEnableDensityScaling = 1;
	//MudHISMC->SetCullDistance(40000);
	FGenerationData CoalData;
	CoalData.Type = EWorldGridType::Coal;
	CoalData.HISM = CoalHISMC;
	CoalData.Frequency = 10;
	CoalData.CutOff = 0.9;
	//CoalData.BaseModelSize = FVector(3.75, 3.75, 3.75);
	CoalData.ScaleWithHeigh = false;
	CoalData.BaseModelSize = FVector(0.23, 0.23, 0.23);
	//CoalData.InvertPlacement = true;
	CoalData.RotateRandomly = true;

	GenerationData.Add(CoalData.Type, CoalData);


	FGenerationData IronData;
	IronData.Type = EWorldGridType::Iron;
	IronData.HISM = IronHISMC;
	IronData.Frequency = 10;
	IronData.CutOff = 0.9;
	//IronData.BaseModelSize = FVector(3.75, 3.75, 3.75);
	IronData.ScaleWithHeigh = false;
	IronData.BaseModelSize = FVector(0.23, 0.23, 0.23);
	//IronData.InvertPlacement = true;
	IronData.RotateRandomly = true;

	GenerationData.Add(IronData.Type, IronData);

	FGenerationData TreeData;
	TreeData.Type = EWorldGridType::Tree;
	TreeData.HISM = TreeHISMC;
	TreeData.Frequency = 7;
	TreeData.CutOff = 0.7;
	TreeData.ScaleWithHeigh = true;
	TreeData.BaseModelSize = FVector(6, 6, 6);
	TreeData.MaxModelSize = FVector(12, 12, 18);
	/*TreeData.RandHeightVariance = 0.5;
	TreeData.RandWidthVariance = .25;
	TreeData.RandXYVariance = 1;
	TreeData.RotateRandomly = true;*/
	GenerationData.Add(TreeData.Type, TreeData);

	FGenerationData RockData;
	RockData.Type = EWorldGridType::Rock;
	RockData.HISM = RockHISMC;
	RockData.Frequency = 7;
	RockData.CutOff = 0.65;
	RockData.ScaleWithHeigh = true;

	RockData.BaseModelSize = FVector(0.15, 0.15, 0.6);
	RockData.MaxModelSize = FVector(0.15, 0.15, 1);
	RockData.RandHeightVariance = 0.4;
	RockData.RandWidthVariance = 0;
	RockData.RandXYVariance = -.2;
	//RockData.RotateRandomly = true;
	//RockData.InvertPlacement = true;
	RockData.ZHeight = 0;
	GenerationData.Add(RockData.Type, RockData);


	FGenerationData MudData;
	MudData.Type = EWorldGridType::Mud;
	MudData.HISM = MudHISMC;
	MudData.Frequency = 7;
	MudData.CutOff = -100;
	MudData.ScaleWithHeigh = false;
	MudData.BaseModelSize = FVector(1, 1, 2);
	MudData.RandHeightVariance = 0;
	MudData.RandWidthVariance = 0;
	MudData.RandXYVariance = 0;
	//MudData.InvertPlacement = true;
	MudData.RotateRandomly = false;
	MudData.ZHeight = -100;
	GenerationData.Add(MudData.Type, MudData);

	FGenerationData GrassData;
	GrassData.Type = EWorldGridType::Grass;
	GrassData.HISM = GrassHISMC;
	GrassData.Frequency = 7;
	GrassData.CutOff = 0.05;
	GrassData.ScaleWithHeigh = false;
	GrassData.BaseModelSize = FVector(1, 1, 2);
	GrassData.RandHeightVariance = 0;
	GrassData.RandWidthVariance = 0;
	GrassData.RandXYVariance = 0;
	GrassData.RotateRandomly = false;
	GrassData.ZHeight = -100;
	GenerationData.Add(GrassData.Type, GrassData);


	FGenerationData WaterData;
	WaterData.Type = EWorldGridType::Water;
	WaterData.HISM = WaterHISMC;
	WaterData.Frequency = 7;
	WaterData.CutOff = 0.6;
	WaterData.ScaleWithHeigh = false;
	WaterData.BaseModelSize = FVector(1, 1, 2);
	WaterData.RandHeightVariance = 0;
	WaterData.RandWidthVariance = 0;
	WaterData.RandXYVariance = 0;
	WaterData.RotateRandomly = false;
	WaterData.ZHeight = 0;
	GenerationData.Add(WaterData.Type, WaterData);


}

// Called when the game starts or when spawned
void ALevelGeneration::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		Seed = FindValidSeed();
		UE_LOG(LogTemp, Warning, TEXT("FindValidSeed Seed: %i"), Seed);

		WaveFrontAlgorithm();
		OnRep_SetSeed();
		CreateCollisionCubes();

		SpawnEnemies();
	}
}

void ALevelGeneration::OnRep_UpdatePositions()
{
	ABDGameState* GameState = Cast<ABDGameState>(GetWorld()->GetGameState());

	//float ServerTime = GameState->GetServerWorldTimeSeconds();
	float CurrentTime = GameState->GetGameTimeSinceCreation();

	FEnemyKeyframe Keyframe = FEnemyKeyframe(EnemyPositions, CurrentTime);
	CurrentEnemyFrame = Keyframe;
}

void ALevelGeneration::UpdateClientEnemies(float DeltaTime)
{
	ABDGameState* GameState = Cast<ABDGameState>(GetWorld()->GetGameState());



	//float ServerTime = GameState->GetServerWorldTimeSeconds();
	float CurrentTime = GameState->GetGameTimeSinceCreation();

	float DelayedTime = CurrentTime - 1.0f - DeltaTime;
	float DifferenceTime = CurrentEnemyFrame.Time - DelayedTime;

	float PercentTime = DeltaTime / DifferenceTime;
	//Clear the old frames apart from the previous one.

	int HISMDifference = FortGolemHISM->GetInstanceCount() < CurrentEnemyFrame.EnemyPositions.Num();

	if (HISMDifference < 0)
	{
		for (int i = 0; i < HISMDifference; i++)
		{
			FortGolemHISM->RemoveInstance(FortGolemHISM->GetInstanceCount() - 1);
		}
	}

	if (HISMDifference > 0)
	{
		FortGolemHISM->AddInstance(FTransform());
	}

	if (CurrentEnemyFrame.Time > DelayedTime)
	{
		for (int i = 0; i < CurrentEnemyFrame.EnemyPositions.Num(); i++)
		{
			
			FTransform InstanceTransform;
			FortGolemHISM->GetInstanceTransform(i, InstanceTransform);
			FVector CurrentPosition = InstanceTransform.GetLocation();
			FVector GoalPosition = FVector(CurrentEnemyFrame.EnemyPositions[i]);

			FRotator CurrentRotation = InstanceTransform.GetRotation().Rotator() + FRotator(0, 90, 0);
			FRotator GoalRotation = UKismetMathLibrary::FindLookAtRotation(CurrentPosition, GoalPosition);
			FRotator NewRotation = CurrentRotation;
			NewRotation.Yaw = CurrentRotation.Yaw + (GoalRotation.Yaw - CurrentRotation.Yaw) * DeltaTime - 90;


			//FVector Location = FMath::Lerp(FVector(CurrentEnemyFrame.EnemyPositions[i]), InstanceTransform.GetLocation(), PercentTime * 100);
			FVector Location = FMath::Lerp(CurrentPosition, GoalPosition, PercentTime);
			FTransform NewTransform;
			NewTransform.SetLocation(Location);
			NewTransform.SetRotation(FQuat(NewRotation));
			FortGolemHISM->UpdateInstanceTransform(i, NewTransform, false, false, false);

			/*if (i == 5)
			{
				UE_LOG(LogTemp, Warning, TEXT("UpdateClientEnemies tick time: CurrentTime %f EnemyFrameTime %f DeltaTime %f DelayedTime %f DifferenceTime %f PercentTime %f CurrentPosition %s GoalPosition %s Location %s DifferenceInPos %s"), CurrentTime, CurrentEnemyFrame.Time, DeltaTime, DelayedTime, DifferenceTime, PercentTime, *CurrentPosition.ToString(), *GoalPosition.ToString(), *Location.ToString(), *FVector(CurrentPosition - Location).ToString());

			}*/
		}
	}




}


void ALevelGeneration::UpdateEnemies(float DeltaTime)
{
	float Count = 0;
	bool LastOne = false;
	for (int i = 0; i < SphereComponents.Num(); i++)
	{
		Count += 0.003;
		if (i == SphereComponents.Num() - 1)
		{
			LastOne = true;
		}
		FTransform Trans;
		FVector Location = SphereComponents[i]->GetComponentLocation();

		int X = -Location.X / (float)GridSize;
		int Y = Location.Y / (float)GridSize;

		int Index = Y * WorldGridSize + X;
		if (Index < VectorMap.Num() - 1 && Index > 0)
		{

			float ForceAmount = 80;
			SphereComponents[i]->AddForce(FVector(-VectorMap[Index].X * ForceAmount, VectorMap[Index].Y * ForceAmount, 0));

			FTransform InstanceTransform;
			FortGolemHISM->GetInstanceTransform(i, InstanceTransform, true);

			//FVector Location = SphereComponents[i]->GetComponentLocation();
			FRotator CurrentRotation = InstanceTransform.GetRotation().Rotator();
			FRotator Rotation = CurrentRotation;
			FVector ComponentVelocity = SphereComponents[i]->GetComponentVelocity();
			FVector Scale;
			if (ComponentVelocity.Size() > 45)
			{
				//Set scale to 5 when faster than 10 velocity.
				Scale = FVector(2);
			}
			else
			{
				//Set scale to for less than 10 speed.
				Scale = FVector(1);
			}

			FRotator VelocityRotation = ComponentVelocity.Rotation() + FRotator(0, -90, 0);
			Rotation.Yaw = Rotation.Yaw + (VelocityRotation.Yaw - CurrentRotation.Yaw) * DeltaTime * FMath::Clamp(ComponentVelocity.Size(), 0.0f, 100.0f) / 100;

			//FVector Scale = FVector(1 + (FMath::Clamp(ComponentVelocity.Size(), 0.0f, 100.0f) / 100) + 1);
			//FVector Scale = FVector(1 + GetWorld()->TimeSeconds / 200);

			Trans.SetLocation(Location);
			Trans.SetRotation(FQuat(Rotation));
			Trans.SetScale3D(Scale);
			/*if (i == 50)
			{
				UE_LOG(LogTemp, Warning, TEXT("Velocity of enemy index found: %f, %f"), ComponentVelocity.Size(), UGameplayStatics::GetRealTimeSeconds(GetWorld()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Velocity of enemy: %i"), SphereComponents.Num());
			}*/
			//FortGolemHISM->UpdateInstanceTransform(i, Trans, false, LastOne, false);
			FortGolemHISM->UpdateInstanceTransform(i, Trans, false, false, false);

			if (EnemyPositions.Num() - 1 > i)
			{
				EnemyPositions[i] = FVector_NetQuantize(Location);
			}
			else
			{
				EnemyPositions.Insert(FVector_NetQuantize(Location), i);
			}
			
		}

	}
	FortGolemHISM->BuildTreeIfOutdated(true, true);
	FortGolemHISM->MarkRenderStateDirty();

	//FVector ComponentVelocity = SphereComponents[i]->GetComponentVelocity();
	//FRotator VelocityRotation = ComponentVelocity.Rotation();

	//Rotation.Yaw += DeltaTime;
	//Rotation.Yaw = Rotation.Yaw + (VelocityRotation.Yaw - CurrentRotation.Yaw) * DeltaTime * FMath::Clamp(ComponentVelocity.Size(), 0.0f, 100.0f)/ 100;
	//Rotation.Yaw = Rotation.Yaw + (VelocityRotation.Yaw - CurrentRotation.Yaw) * DeltaTime;

	



}

void ALevelGeneration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority)
	{
		UpdateEnemies(DeltaTime);
	}
	else
	{
		UpdateClientEnemies(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("CLIENT"));

	}
}

void ALevelGeneration::SpawnEnemies()
{
	int Number = 0;
	for (int i = 0; i < CollisionGrid.Num(); i++)
	{
		if (CollisionGrid[i] != 1)
		{
			//int X = (i % WorldGridSize) * GridSize;
			////y value
			//int Y = (i / WorldGridSize) * GridSize;

			int Remainder = i % WorldGridSize;
			int Quotient = i / WorldGridSize;
			int X = Remainder * GridSize + GridSize / 2;
			int Y = Quotient * GridSize + GridSize / 2;

			USphereComponent * SphereComponent = NewObject<USphereComponent>(this);
			//SphereComponent->SetConstraintMode(EDOFMode::XYPlane);
			SphereComponent->GetBodyInstance()->bLockZTranslation = true;
			SphereComponent->GetBodyInstance()->SetEnableGravity(false);
			SphereComponent->GetBodyInstance()->bLockXRotation = true;
			SphereComponent->GetBodyInstance()->bLockYRotation = true;
			SphereComponent->GetBodyInstance()->bLockZRotation = true;
			SphereComponent->GetBodyInstance()->SetMaxDepenetrationVelocity(1);
			SphereComponent->GetBodyInstance()->SetMassScale(0.1);
			SphereComponent->GetBodyInstance()->SetUseCCD(false);


			SphereComponent->SetLinearDamping(0.5f);
			
			SphereComponent->SetWorldScale3D(FVector(0.7f));
			UPhysicalMaterial* PhysMat = NewObject<UPhysicalMaterial>(this);
			PhysMat->Friction = 0;
			SphereComponent->SetPhysMaterialOverride(PhysMat);
			SphereComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			SphereComponent->SetCollisionResponseToAllChannels(ECR_Block);
			SphereComponent->SetSimulatePhysics(true);
			SphereComponent->SetWorldLocation(FVector(-X, Y, 15));
			//SphereComponent->SetHiddenInGame(false);
			SphereComponent->RegisterComponent();
			SphereComponents.Add(SphereComponent);

			FTransform Trans = FTransform(FRotator(0), FVector(0));


			
			FortGolemHISM->AddInstance(FTransform(Trans));
			if (EnemyPositions.Num() - 1 > i)
			{
				//EnemyPositions[i] = FVector_NetQuantize(Trans.GetLocation());
			}
			else
			{
				//EnemyPositions.Insert(FVector_NetQuantize(Trans.GetLocation()), i);
				EnemyPositions.AddUninitialized();
			}

			Number++;
		}
		if (Number == 2000) break;
	}
}

void ALevelGeneration::CreateCollisionCubes()
{
	for (int i = 0; i < CollisionGrid.Num(); i++)
	{
		if (CollisionGrid[i] == 1)
		{
			//int X = (i % WorldGridSize) * GridSize;
			////y value
			//int Y = (i / WorldGridSize) * GridSize;

			int Remainder = i % WorldGridSize;
			int Quotient = i / WorldGridSize;
			int X = Remainder * GridSize + GridSize / 2;
			int Y = Quotient * GridSize + GridSize / 2;

			UBoxComponent * BoxComponent = NewObject<UBoxComponent>(this);
			UPhysicalMaterial * PhysMat = NewObject<UPhysicalMaterial>(this);
			BoxComponent->SetBoxExtent(FVector(50.0f));
			PhysMat->Friction = 0;
			BoxComponent->SetPhysMaterialOverride(PhysMat);
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
			BoxComponent->SetSimulatePhysics(false);
			BoxComponent->SetWorldLocation(FVector(-X, Y, 50));
			//BoxComponent->SetHiddenInGame(false);
			BoxComponent->RegisterComponent();
		}
	}
}

int ALevelGeneration::FindValidSeed()
{
	bool IsValidSeed = false;

	int TempSeed = 0;
	int SeedCount = 0;


	while (IsValidSeed == false)
	{
		TempSeed = FMath::RandRange((int32)1, (int32)99999);

		GenerateGrids(TempSeed);

		IsValidSeed = TestGrids();
		SeedCount++;
	}
	UE_LOG(LogTemp, Warning, TEXT("FindValidSeed seedcount: %i"), SeedCount);

	return TempSeed;

}

//We dont deal with cases where the buildings are near the edge of a grid because we dont allow buildings to be built near the edges
TArray<EWorldGridType> ALevelGeneration::GetGridPortion(TArray<EWorldGridType> AGrid, int AFromIndex, int AnXSize, int AYSize)
{
	TArray<EWorldGridType> ReturnArray;

	for (int i = 0; i < AYSize; i++)
	{
		for (int j = 0; j < AnXSize; j++)
		{
			int CurrentIndex = AFromIndex + i * WorldGridSize + j;

			ReturnArray.Add(AGrid[CurrentIndex]);
		}
	}
	return ReturnArray;
}

//inputting size 1 would get the 8 tiles around the AFromIndex and also the index itself
TArray<EWorldGridType> ALevelGeneration::GetGridPortionAround(TArray<EWorldGridType> AGrid, int AFromIndex, int ASize)
{
	int InputFromIndex = AFromIndex - ASize - ASize * WorldGridSize;
	int InputSize = 1 + ASize * 2;
	return GetGridPortion(AGrid, InputFromIndex, InputSize, InputSize);
}

int ALevelGeneration::GetGridIndex(FVector APostion)
{
	int X = 0;
	int Y = 0;
	X = (int)-APostion.X / GridSize;
	Y = (int)APostion.Y / GridSize;

	return Y * WorldGridSize + X;
}

int ALevelGeneration::GetGridIndex(int AnX, int AY)
{
	return WorldGridSize * AY + AnX;
}

float ALevelGeneration::AStarPathLength(TArray<uint8> AMaze, int AMazeLength, int AStart, int AnEnd)
{
	FASGraph Graph;
	Graph.Nodes = AMaze;
	Graph.GridSize = AMazeLength;
	FGraphAStar<FASGraph> GraphAStar = FGraphAStar<FASGraph>(Graph);
	TArray<int32> OutPath;

	FQueryFilter Qf;
	Qf.MapSize = AMazeLength;
	Qf.Maze = AMaze;
	GraphAStar.FindPath(AStart, AnEnd, Qf, OutPath);
	
	float Cost = 0;
	if (OutPath.Num() > 0)
	{
		Cost = Qf.GetTraversalCost(OutPath[0], OutPath[OutPath.Num() - 1]);

	}
	if (OutPath.Num() > 0)
	{
		for (int i = 1; i < OutPath.Num(); i++)
		{
			Cost += Qf.GetTraversalCost(OutPath[i - 1], OutPath[i]);
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("AStarPathLength: %f"), Cost);


	return Cost;
}



bool ALevelGeneration::TestGrids()
{
	//CreateNavGrid
	

	GenerateCollisionGrid();

	if (CollisionGrid[0] == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestGrids: False Start"));
		return false;
	}


	int MiddleNode = ((WorldGridSize) * (WorldGridSize)) / 2 + (WorldGridSize / 2);

	//float PathLength = AStarPathLength(CollisionGrid, CollisionGrid.Num(), 0, MiddleNode);
	float PathLength = AStarPathLength(CollisionGrid, WorldGridSize, 0, MiddleNode);

	if (CollisionGrid[MiddleNode] == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestGrids: NavGrid[MiddleNode] == 1"));
		return false;
	}

	return (PathLength > 0);
	//return (PathLength == 0);
}

void ALevelGeneration::GenerateCollisionGrid()
{
	TArray<uint8> NewGrid;
	NewGrid.SetNum((WorldGridSize) * (WorldGridSize));
	CollisionGrid = NewGrid;
	for (int i = 0; i < ResourcesGrid.Num(); i++)
	{
		if (GroundGrid[i] == EWorldGridType::Water)
		{
			CollisionGrid[i] = 1;
		}

		if (ResourcesGrid[i] == EWorldGridType::Rock || ResourcesGrid[i] == EWorldGridType::Tree)
		{
			CollisionGrid[i] = 1;
		}
	}
}

void ALevelGeneration::GenerateGrids(int ASeed)
{
	ResourcesGrid.Empty();
	ResourcesElevation.Empty();
	GroundGrid.Empty();
	GroundElevation.Empty();

	FGenerationData CoalData = (*GenerationData.Find(EWorldGridType::Coal));
	FGenerationData IronData = (*GenerationData.Find(EWorldGridType::Iron));
	FGenerationData TreeData = (*GenerationData.Find(EWorldGridType::Tree));
	FGenerationData RockData = (*GenerationData.Find(EWorldGridType::Rock));
	FGenerationData WaterData = (*GenerationData.Find(EWorldGridType::Water));
	FGenerationData MudData = (*GenerationData.Find(EWorldGridType::Mud));
	FGenerationData GrassData = (*GenerationData.Find(EWorldGridType::Grass));

	TArray<float> CoalGrid = CreateSimplexGrid(ASeed, WorldGridSize, CoalData.Frequency);
	TArray<float> IronGrid = CreateSimplexGrid(ASeed + 1, WorldGridSize, IronData.Frequency);

	TArray<float> TreeGrid = CreateSimplexGrid(ASeed + 2, WorldGridSize, TreeData.Frequency);
	TArray<float> RockGrid = CreateSimplexGrid(ASeed + 3, WorldGridSize, RockData.Frequency);
	TArray<float> WaterGrid = CreateSimplexGrid(ASeed + 4, WorldGridSize, WaterData.Frequency);

	//Grass uses the tree grid

	//Mud doesnt need a grid because its default
	TArray<float> MudGrid;
	MudGrid.AddZeroed(WorldGridSize * WorldGridSize);




	//Coal
	AddGridToGrid(CoalGrid, ResourcesGrid, ResourcesElevation, CoalData.CutOff, true, CoalData.Type);

	//Iron
	AddGridToGrid(IronGrid, ResourcesGrid, ResourcesElevation, IronData.CutOff, true, IronData.Type);

	//Trees
	AddGridToGrid(TreeGrid, ResourcesGrid, ResourcesElevation, TreeData.CutOff, true, TreeData.Type);

	//Rocks
	AddGridToGrid(RockGrid, ResourcesGrid, ResourcesElevation, RockData.CutOff, true, RockData.Type);

	//Mud
	AddGridToGrid(TreeGrid, GroundGrid, GroundElevation, MudData.CutOff, true, MudData.Type);

	//Grass
	AddGridToGrid(TreeGrid, GroundGrid, GroundElevation, GrassData.CutOff, true, GrassData.Type);

	//Water affects both ground and Resources
	AddGridToGrid(WaterGrid, GroundGrid, GroundElevation, WaterData.CutOff, true, WaterData.Type);
	AddGridToGrid(WaterGrid, ResourcesGrid, GroundElevation, WaterData.CutOff, true, WaterData.Type);
}




void ALevelGeneration::MakeMiniMapTexture()
{
	TArray<FColor> RawData;
	RawData.SetNum((WorldGridSize) * (WorldGridSize));


	TArray<uint8> NavGrid;
	int NavGridSize = WorldGridSize;
	NavGrid.SetNum((NavGridSize) * (NavGridSize));


	for (int i = 0; i < ResourcesGrid.Num(); i++)
	{
		if (GroundGrid[i] == EWorldGridType::Grass)
		{
			RawData[i] = FColor(0, 204, 0, 255);
		}
		if (GroundGrid[i] == EWorldGridType::Mud)
		{
			RawData[i] = FColor(153, 76, 0, 255);
		}


		if (ResourcesGrid[i] == EWorldGridType::Water)
		{
			RawData[i] = FColor(0, 47, 141, 255);
		}
		if (ResourcesGrid[i] == EWorldGridType::Rock)
		{
			RawData[i] = FColor(128, 128, 128, 255);
		}
		if (ResourcesGrid[i] == EWorldGridType::Tree)
		{
			RawData[i] = FColor(51, 102, 0, 255);
		}




	}

	MiniMapTexture = UTexture2D::CreateTransient(256, 256);
	FTexture2DMipMap& Mip = MiniMapTexture->PlatformData->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, RawData.GetData(), 256 * 256 * sizeof(FColor));
	Mip.BulkData.Unlock();
	MiniMapTexture->UpdateResource();
}

void ALevelGeneration::OnRep_SetSeed()
{
	ENetRole MyRole = Role;
	if (Seed != 0 && HasGenerated == false)
	{
		if (Role != ROLE_Authority)
		{
			//we have already done this on the host as part of finding a valid seed
			GenerateGrids(Seed);
		}
		GenerateWorld();
		MakeMiniMapTexture();
		OnGenerateWorld.Broadcast();
	}
}



void ALevelGeneration::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALevelGeneration, Seed);
	DOREPLIFETIME(ALevelGeneration, EnemyPositions);
	/*DOREPLIFETIME(ALevelGeneration, RockHISMC);
	DOREPLIFETIME(ALevelGeneration, MudHISMC);
	DOREPLIFETIME(ALevelGeneration, GrassHISMC);
	DOREPLIFETIME(ALevelGeneration, CoalHISMC);
	DOREPLIFETIME(ALevelGeneration, IronHISMC);*/
}

FVector2D ALevelGeneration::GetVectorFromNeighbors(TArray<uint8> ACollisionGrid, TArray<uint16> HeatMap, int AnIndex)
{
	FVector2D ReturnVector;
	//x value
	int X = (AnIndex % WorldGridSize) + 1;
	//y value
	int Y = (AnIndex) / WorldGridSize + 1;

	bool Up = (Y < WorldGridSize);
	bool Down = (Y > 1);
	bool Left = (X > 1);
	bool Right = (X < WorldGridSize);

	int UpInt = 0;
	int DownInt = 0;
	int LeftInt = 0;
	int RightInt = 0;


	int UpIndex = AnIndex + WorldGridSize;

	if (Up && ACollisionGrid[UpIndex] == 0)
	{
		UpInt = HeatMap[UpIndex];
	}
	else
	{
		UpInt = HeatMap[AnIndex];
	}

	int RightIndex = AnIndex + 1;
	if (Right && ACollisionGrid[RightIndex] == 0)
	{
		RightInt = HeatMap[RightIndex];
	}
	else
	{
		RightInt = HeatMap[AnIndex];
	}

	int DownIndex = AnIndex - WorldGridSize;
	if (Down && ACollisionGrid[DownIndex] == 0)
	{
		DownInt = HeatMap[DownIndex];
	}
	else
	{
		DownInt = HeatMap[AnIndex];
	}

	int LeftIndex = AnIndex - 1;
	if (Left && ACollisionGrid[LeftIndex] == 0)
	{
		LeftInt = HeatMap[LeftIndex];
	}
	else
	{
		LeftInt = HeatMap[AnIndex];
	}

	if (UpInt == DownInt && LeftInt == RightInt)
	{
		if (UpInt < HeatMap[AnIndex])
		{
			DownInt--;
		}
		else if (LeftInt < HeatMap[AnIndex])
		{
			LeftInt--;
		}
	}


	ReturnVector.X = LeftInt - RightInt;
	ReturnVector.Y = DownInt - UpInt;

	ReturnVector = ReturnVector.GetSafeNormal();

	//Check if local optima occurs by checking if we are walking into a wall. if we are then just walk up or right.
	//Left and right https://gamedevelopment.tutsplus.com/tutorials/understanding-goal-based-vector-field-pathfinding--gamedev-9007
	if (((ReturnVector.X == -1 && (!Left || ACollisionGrid[AnIndex - 1] == 1)) || (ReturnVector.X == 1 && (!Right || ACollisionGrid[AnIndex + 1] == 1))) && ReturnVector.Y == 0)
	{
		ReturnVector.X = 0;
		ReturnVector.Y = 1;
	}
	//check up and down
	else if (((ReturnVector.Y == -1 && (!Down || ACollisionGrid[DownIndex] == 1)) || (ReturnVector.Y == 1 && (!Up || ACollisionGrid[UpIndex] == 1))) && ReturnVector.X == 0)
	{
		ReturnVector.X = 1;
		ReturnVector.Y = 0;
	}



	return ReturnVector;
}


TArray<int> ALevelGeneration::GetNeighbourList(TArray<uint8> ACollisionGrid, TArray<bool> ACheckedMap, int AnIndex)
{
	TArray<int> Neighbors;
	//x value
	int X = (AnIndex % WorldGridSize) + 1;
	//y value
	int Y = (AnIndex) / WorldGridSize + 1;

	bool Up = (Y > 1);
	bool Down = (Y < WorldGridSize);
	bool Left = (X > 1);
	bool Right = (X < WorldGridSize);

	

	if (Up && ACollisionGrid[AnIndex - WorldGridSize] == 0)
	{
		int ReturnIndex = AnIndex - WorldGridSize;
		if (!ACheckedMap[ReturnIndex])
		{
			Neighbors.Add(AnIndex - WorldGridSize);
		}
	}

	if (Right && ACollisionGrid[AnIndex + 1] == 0)
	{
		int ReturnIndex = AnIndex + 1;
		if (!ACheckedMap[ReturnIndex])
		{
			Neighbors.Add(ReturnIndex);
		}
	}

	if (Down && ACollisionGrid[AnIndex + WorldGridSize] == 0)
	{
		int ReturnIndex = AnIndex + WorldGridSize;
		if (!ACheckedMap[ReturnIndex])
		{
			Neighbors.Add(ReturnIndex);
		}
	}

	if (Left && ACollisionGrid[AnIndex - 1] == 0)
	{
		int ReturnIndex = AnIndex - 1;
		if (!ACheckedMap[ReturnIndex])
		{
			Neighbors.Add(ReturnIndex);
		}
	}

	return Neighbors;
}

void ALevelGeneration::WaveFrontAlgorithm()
{
	TArray<int> ShouldCheckBucket;
	TArray<bool> CheckedMap;

	TArray<uint16> HeatMap;
	HeatMap.SetNum(WorldGridSize* WorldGridSize);
	CheckedMap.SetNum(WorldGridSize * WorldGridSize);
	int MiddleNode = (WorldGridSize * WorldGridSize) / 2 + (WorldGridSize / 2);
	HeatMap[MiddleNode] = 0;

	ShouldCheckBucket.Add(MiddleNode);


	while (ShouldCheckBucket.Num() > 0)
	{
		int X = ShouldCheckBucket[0];
		ShouldCheckBucket.RemoveAt(0);
			

		CheckedMap[X] = true;
		//get the neighbors that havent been checked and arent collsion.
		TArray<int> Neighbors = GetNeighbourList(CollisionGrid, CheckedMap, X);
		//Update heatmap for neighbors
		for (int i = 0; i < Neighbors.Num(); i++)
		{
			if (!ShouldCheckBucket.Contains(Neighbors[i]))
			{
				HeatMap[Neighbors[i]] = HeatMap[X] + 1;
				ShouldCheckBucket.Add(Neighbors[i]);
			}
		}
	}
	for (int i = 0; i < CollisionGrid.Num(); i++)
	{
		if (CollisionGrid[i] == 0)
		{
			VectorMap.Add(GetVectorFromNeighbors(CollisionGrid, HeatMap, i));
		}
		else
		{
			VectorMap.Add(FVector2D(0, 0));
		}
	}

	/*int Count = 0;
	for (int i = 0; i < VectorMap.Num(); i++)
	{
		int Remainder = i % WorldGridSize;
		int Quotient = i / WorldGridSize;
		int X = Remainder * GridSize + GridSize / 2;
		int Y = Quotient * GridSize + GridSize / 2;

		FVector LineStart = FVector(-X , Y, 1);
		FVector LineEnd = FVector(-X -VectorMap[i].X * 50, Y + VectorMap[i].Y * 50, 1);
		UKismetSystemLibrary::DrawDebugArrow(this, LineStart, LineEnd, 200, FLinearColor(FColor::Green), 1000.0f, 4.0f);
		Count++;
		if (Count == 10000)
		{
			break;
		}
	}*/



}

void ALevelGeneration::GenerateWorld()
{
	if (HasGenerated == false)
	{
		
		FGenerationData CoalData = (*GenerationData.Find(EWorldGridType::Coal));
		FGenerationData IronData = (*GenerationData.Find(EWorldGridType::Iron));
		FGenerationData TreeData = (*GenerationData.Find(EWorldGridType::Tree));
		FGenerationData RockData = (*GenerationData.Find(EWorldGridType::Rock));
		FGenerationData WaterData = (*GenerationData.Find(EWorldGridType::Water));
		FGenerationData MudData = (*GenerationData.Find(EWorldGridType::Mud));
		FGenerationData GrassData = (*GenerationData.Find(EWorldGridType::Grass));

		SpawnMeshes(CoalData, ResourcesGrid, ResourcesElevation);
		SpawnMeshes(IronData, ResourcesGrid, ResourcesElevation);

		SpawnMeshes(MudData, GroundGrid, GroundElevation);
		SpawnMeshes(GrassData, GroundGrid, GroundElevation);
		//SpawnMeshes(WaterData, GroundGrid, GroundElevation);
		SpawnMeshes(TreeData, ResourcesGrid, ResourcesElevation);
		SpawnMeshes(RockData, ResourcesGrid, ResourcesElevation);
		HasGenerated = true;
	}

}


void ALevelGeneration::SpawnMeshes(FGenerationData AGenerationData, TArray<EWorldGridType> AFromGrid, TArray<float> AFromElevation)
{

	int Count = 0;
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
			int X = Remainder * GridSize + GridSize / 2;
			int Y = Quotient * GridSize + GridSize / 2;

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
			Count++;
			AGenerationData.HISM->AddInstance(FTransform(Trans));
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("SpawnMeshes %s: Number %i, out of %i"), *AGenerationData.HISM->GetName(), Count, AFromGrid.Num());

}

void ALevelGeneration::AddGridToGrid(TArray<float> AFromGrid, TArray<EWorldGridType>& AToGrid, TArray<float>& AToElevation, float ACutOff, bool AMoreThan, EWorldGridType AType)
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


