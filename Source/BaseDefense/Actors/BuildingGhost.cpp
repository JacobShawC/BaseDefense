// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingGhost.h"
#include "BDGameInstance.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
ABuildingGhost::ABuildingGhost()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetSimulatePhysics(false);

	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComponent->SetReceivesDecals(false);
}



void ABuildingGhost::Initialise(EBuilding ABuilding, bool AReachable, bool ABuildable)
{
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());

	SetBuilding(ABuilding);

	MeshComponent->SetScalarParameterValueOnMaterials(FName(TEXT("ColourAmount")), 1.f);

	SetReachable(AReachable);
	SetBuildable(ABuildable);

	Initialised = true;
}


void ABuildingGhost::SetReachable(bool AReachable)
{
	if (AReachable != Reachable || !Initialised)
	{
		Reachable = AReachable;

		float LightOrDark = 0;

		if (!Reachable)
		{
			LightOrDark = 1;
		}
		if (MeshComponent)
		{
			MeshComponent->SetScalarParameterValueOnMaterials(FName(TEXT("LightOrDark")), LightOrDark);
		}
	}
	
}

void ABuildingGhost::SetBuildable(bool ABuildable)
{
	if (ABuildable != Buildable || !Initialised)
	{
		Buildable = ABuildable;
		float GreenOrRed = 0;

		if (!Buildable)
		{
			GreenOrRed = 1;
		}
		if (MeshComponent)
		{
			MeshComponent->SetScalarParameterValueOnMaterials(FName(TEXT("GreenOrRed")), GreenOrRed);
		}
	}
}

void ABuildingGhost::SetBuilding(EBuilding ABuilding)
{
	if (ABuilding != Building || !Initialised)
	{
		Building = ABuilding;
		FBuildingData BuildingData;
		if (GameInstance)
		{
			BuildingData = *GameInstance->Buildings.Find(ABuilding);
			UStaticMesh* Mesh = nullptr;
			Mesh = BuildingData.Mesh;
			if (Mesh)
			{
				MeshComponent->SetStaticMesh(Mesh);
				FVector Scale = FVector(BuildingData.MeshScale);
				MeshComponent->SetRelativeScale3D(Scale);
			}
		}
	}
}