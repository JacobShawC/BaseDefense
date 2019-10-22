// Fill out your copyright notice in the Description page of Project Settings.


#include "HISMManager.h"
#include <Components/HierarchicalInstancedStaticMeshComponent.h>
#include <Engine/StaticMesh.h>

// Sets default values
AHISMManager::AHISMManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeshPool = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("MeshPool"));

}


void AHISMManager::FinishUpdates()
{
	MeshPool->MarkRenderStateDirty();
}

void AHISMManager::SetStaticMesh(UStaticMesh* AMesh)
{
	MeshPool->SetStaticMesh(AMesh);
}

//We can only delete the last instance so therefore we swap the chosen actor with the last actor and then delete the last actor.
void AHISMManager::DestroyIM(uint32 AUnitID)
{
	FIMInstance ActorInstance = IDToInstanceMapping[AUnitID];
	uint32 LastUnitID = IDBuffer[IDBuffer.Num() - 1];

	//Delete the last instance
	uint32 LastInstanceIndex = IDToInstanceMapping[LastUnitID].Index;
	MeshPool->RemoveInstance(LastInstanceIndex);

	//Swap the mappings' instance id
	IDBuffer[ActorInstance.Index] = LastUnitID;
	IDToInstanceMapping[LastUnitID] = ActorInstance;

	//Delete the new mapping.
	IDToInstanceMapping.Remove(AUnitID);

	//Delete the last ID in the buffer.
	IDBuffer.RemoveAt(IDBuffer.Num() - 1);

}

void AHISMManager::SpawnIM(uint32 AUnitID, FTransform AnInitialTransform)
{
	//Add the unit to the idbuffer (which mirrors the HISM instances)
	IDBuffer.Add(AUnitID);
	int32 InstanceIndex = MeshPool->AddInstanceWorldSpace(AnInitialTransform);
	IDToInstanceMapping.Add(AUnitID, FIMInstance(InstanceIndex));
}

void AHISMManager::TransformIM(uint32 AUnitID, FTransform ATransform)
{
	uint32 InstanceIndex = IDToInstanceMapping[AUnitID].Index;
	IDBuffer.Add(InstanceIndex);
	MeshPool->UpdateInstanceTransform(InstanceIndex, ATransform, true, false, false);
}

