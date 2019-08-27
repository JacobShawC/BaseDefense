// Fill out your copyright notice in the Description page of Project Settings.


#include "HISMManager.h"
#include <Components/HierarchicalInstancedStaticMeshComponent.h>

// Sets default values
AHISMManager::AHISMManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}


void AHISMManager::FinishUpdates()
{
	MeshPool->MarkRenderStateDirty();
}

//We can only delete the last instance so therefore we swap the chosen actor with the last actor and then delete the last actor.
void AHISMManager::DestroyIM(uint32 AnActorID)
{
	FIMInstance ActorInstance = IDToInstanceMapping[AnActorID];
	uint32 LastActorID = IDBuffer[IDBuffer.Num() - 1];

	//Delete the last instance
	uint32 LastInstanceIndex = IDToInstanceMapping[LastActorID].Index;
	MeshPool->RemoveInstance(LastInstanceIndex);



	//Swap the mappings' instance id
	IDBuffer[ActorInstance.Index] = LastActorID;
	IDToInstanceMapping[LastActorID] = ActorInstance;

	//Delete the new mapping.
	IDToInstanceMapping.Remove(AnActorID);

	//Delete the last ID in the buffer.
	IDBuffer.RemoveAt(IDBuffer[IDBuffer.Num() - 1]);

}

uint32 AHISMManager::SpawnIM(FTransform AnInitialTransform)
{
	ActorIDCount++;

	IDBuffer.Add(ActorIDCount);
	int32 InstanceIndex = MeshPool->AddInstanceWorldSpace(AnInitialTransform);
	IDToInstanceMapping.Add(ActorIDCount, FIMInstance(InstanceIndex));
	return ActorIDCount;
}	

void AHISMManager::TransformIM(uint32 AnActorID, FTransform ATransform)
{
	uint32 InstanceIndex = IDToInstanceMapping[AnActorID].Index;
	IDBuffer.Add(InstanceIndex);
	MeshPool->UpdateInstanceTransform(InstanceIndex, ATransform, true, false, false);
}

