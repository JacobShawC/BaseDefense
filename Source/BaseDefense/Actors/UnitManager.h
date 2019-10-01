// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HISMManager.h"
#include "StructLibrary.h"
#include <Engine/NetSerialization.h>
#include "UnitManager.generated.h"
//Here we store an array of frames for each unit. This is updated every time the UnitFrames array is replicated.
DECLARE_MULTICAST_DELEGATE_OneParam(FUnitFrameItemChangedOrAddedDelegate, FUnitFrameItem&);
DECLARE_MULTICAST_DELEGATE_OneParam(FUnitFrameItemRemovedDelegate, FUnitFrameItem&);



USTRUCT()
struct FUnitFrameItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	float CurrentTime = 0;

	UPROPERTY(NotReplicated)
	bool Dead = false;

	UPROPERTY()
	uint32 UnitID;

	UPROPERTY()
	EGameUnit UnitType = EGameUnit::None;

	UPROPERTY()
	uint16 XPosition = 0;

	UPROPERTY()
	uint16 YPosition = 0;

	FORCEINLINE bool operator==(const FUnitFrameItem& AnOtherItem) const
	{
		return (UnitID == AnOtherItem.UnitID && UnitType == AnOtherItem.UnitType && XPosition == AnOtherItem.XPosition && YPosition == AnOtherItem.YPosition);
	}
	
	FORCEINLINE bool operator!=(const FUnitFrameItem& AnOtherItem) const
	{
		return !(this->operator==(AnOtherItem));
	}

	/**
	 * Optional functions you can implement for client side notification of changes to items;
	 * Parameter type can match the type passed as the 2nd template parameter in associated call to FastArrayDeltaSerialize
	 *
	 * NOTE: It is not safe to modify the contents of the array serializer within these functions, nor to rely on the contents of the array
	 * being entirely up-to-date as these functions are called on items individually as they are updated, and so may be called in the middle of a mass update.
	 */
	void PreReplicatedRemove(const struct FUnitFrame& InArraySerializer);
	void PostReplicatedAdd(const struct FUnitFrame& InArraySerializer);
	void PostReplicatedChange(const struct FUnitFrame& InArraySerializer);

	// Optional: debug string used with LogNetFastTArray logging
	//FString GetDebugString();

};

/** Step 2: You MUST wrap your TArray in another struct that inherits from FFastArraySerializer */
USTRUCT()
struct FUnitFrame : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	float CurrentTime = 0;

	UPROPERTY()
	TArray<FUnitFrameItem>	Items;	/** Step 3: You MUST have a TArray named Items of the struct you made in step 1. */


	FUnitFrameItemChangedOrAddedDelegate OnItemAddedOrChanged;
	
	FUnitFrameItemRemovedDelegate OnItemRemoved;

	void UpdateUnit(FUnitInstance& AUnitInstance, FVector AVector, float ACurrentTime, int AnIndex)
	{
		FUnitFrameItem Item;
		Item.UnitID = AUnitInstance.ID;
		Item.XPosition = AVector.X;
		Item.YPosition = AVector.Y;
		Item.UnitType = AUnitInstance.Type;

		// if a unit has been killed or removed we remove it from the item list. This is done by comparing the unit list and item list one by one.
		while (Items.Num() > AnIndex && Items[AnIndex].UnitID != Item.UnitID)
		{
			RemoveItem(AnIndex);
			CurrentTime = ACurrentTime;
		}

		//if the units list is larger than the items list we need to add a new item. otherwise we simply change an existing item.
		if (Items.Num() <= AnIndex)
		{
			AddItem(Item);
			CurrentTime = ACurrentTime;
		}
		else
		{
			// we only set the current time if the item has been changed.
			if (ChangeItem(Item, AnIndex))
			{
				CurrentTime = ACurrentTime;
			}
		}
	}

	void AddItem(FUnitFrameItem Item)
	{
		this->MarkItemDirty(Items.Add_GetRef(Item));
	}

	//returns whether the item was changed or not.
	bool ChangeItem(FUnitFrameItem Item, int AnIndex)
	{
		if (Item != Items[AnIndex])
		{
			Items[AnIndex].UnitID = Item.UnitID;
			Items[AnIndex].UnitType = Item.UnitType;
			Items[AnIndex].XPosition = Item.XPosition;
			Items[AnIndex].YPosition = Item.YPosition;

			this->MarkItemDirty(Items[AnIndex]);
			return true;
		}
		return false;
	}

	void RemoveItem(int AnIndex)
	{
		Items.RemoveAt(AnIndex);
		MarkArrayDirty();
	}

		/** Step 4: Copy this, replace example with your names */
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FUnitFrameItem, FUnitFrame>(Items, DeltaParms, *this);
	}
};
template<>
struct TStructOpsTypeTraits< FUnitFrame > : public TStructOpsTypeTraitsBase2< FUnitFrame >
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

UCLASS()
class BASEDEFENSE_API AUnitManager : public AActor
{
	GENERATED_BODY()



public:
	// Sets default values for this actor's properties
	AUnitManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	uint32 UnitIDCount = 0;
	void SpawnUnit(EGameUnit AUnit, FTransform AnInitialTransform);

	void ForceUnit(uint32 AUnitID, FVector Direction, float AnAmount);

	void SetupAllHISMS();

	void SetupHISM(FUnitData AUnitData);

	void UpdateCurrentFrame();

	void PerformActions();
	void SetActionToDefault(FUnitInstance* AUnitInstance);
	
	void PathTowardsPosition(FUnitInstance AnInstance);

	UFUNCTION()
	void OnItemChangedOrAdded(FUnitFrameItem& AnItem);
	
	UFUNCTION()
	void OnItemRemoved(FUnitFrameItem& AnItem);

	UFUNCTION()
	void OnRangeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void OnRep_CurrentFrame();

	void TestSpawn();

	void Tick(float DeltaTime);

	void UpdateHISMPositions();

	TMap<uint32, FUnitInstance> UnitIDMap;

	TMap<EGameUnit, TArray<FUnitInstance>> UnitTypeMap;
	TMap<EGameUnit, FUnitData> UnitDataMap;


protected:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentFrame)
	FUnitFrame CurrentFrame;

	class ALevelGeneration* LevelGenerationActor = nullptr;
public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TMap<EGameUnit, AHISMManager*> HISMManagers;

	TMap<uint32, TArray<FUnitFrameItem>> UnitFrames;
};