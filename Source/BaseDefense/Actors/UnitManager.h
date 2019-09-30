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

	float CurrentTime = 0;

	bool Dead = false;

	UPROPERTY()
	uint32 UnitID;

	UPROPERTY()
	uint16 XPosition = 0;

	UPROPERTY()
	uint16 YPosition = 0;



	/*bool operator == (const FUnitFrameItem AnOtherItem)
	{
		if (UnitID == AnOtherItem.UnitID && XPosition == AnOtherItem.XPosition && YPosition == AnOtherItem.YPosition)
		{
			return true;
		}
		else return false;
	}*/

	FORCEINLINE bool operator==(const FUnitFrameItem& AnOtherItem) const
	{
		return (UnitID == AnOtherItem.UnitID && XPosition == AnOtherItem.XPosition && YPosition == AnOtherItem.YPosition);
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

	TMap<uint32, FUnitFrameItem*> IDMap;

	FUnitFrameItemChangedOrAddedDelegate OnItemAddedOrChanged;
	
	FUnitFrameItemRemovedDelegate OnItemRemoved;

	void AddItem(uint32 ItemID, FVector APosition, float ACurrentTime)
	{
		FUnitFrameItem Item;
		Item.XPosition = APosition.X;
		Item.YPosition = APosition.Y;
		CurrentTime = ACurrentTime;
		this->MarkItemDirty(Item);
		IDMap.Add(ItemID, &Item);
		Items.Add(Item);
	}

	void ChangeItem(uint32 ItemID, FVector APosition, float ACurrentTime)
	{
		FUnitFrameItem* Item = *IDMap.Find(ItemID);
		if (Item != nullptr)
		{
			Item->XPosition = APosition.X;
			Item->YPosition = APosition.Y;
			CurrentTime = ACurrentTime;
			this->MarkItemDirty(*Item);
			Items.Add(*Item);
		}
	}

	void RemoveItem(uint32 ItemID)
	{
		FUnitFrameItem* Item = IDMap.FindAndRemoveChecked(ItemID);
		Items.Remove(*Item);
		this->MarkArrayDirty();
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