// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructLibrary.h"
#include "BuildingGhost.generated.h"

UCLASS()
class BASEDEFENSE_API ABuildingGhost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingGhost();

	void Initialise(EBuilding ABuilding, bool AReachable, bool ABuildable, bool AMinable);

	void SetReachable(bool AReachable);
	void SetBuildable(bool ABuildable);
	void SetMineable(bool AMineable);

	void SetBuilding(EBuilding ABuilding);

protected:

public:
	class UStaticMeshComponent* MeshComponent;
	EBuilding Building;
	bool Reachable = false;
	bool Buildable = false;
	bool Initialised = false;

protected:
	class UBDGameInstance* GameInstance = nullptr;
};
