// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction.h"
#include "StructLibrary.h"
#include "Building.generated.h"



UENUM()
enum class EBuildingInteractionType : uint8
{
	None 			UMETA(DisplayName = "None"),
	Construct		UMETA(DisplayName = "Construct"),
	Upgrade 		UMETA(DisplayName = "Upgrade"),
	Destroy 		UMETA(DisplayName = "Destroy"),
	Sell 			UMETA(DisplayName = "Destroy"),
};

USTRUCT()
struct FBuildingInteraction
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	class APlayerChar* Interactor = nullptr;

	UPROPERTY()
	EBuildingInteractionType Type = EBuildingInteractionType::None;

	UPROPERTY()
	EBuilding Building = EBuilding::None;

	UPROPERTY()
	float Duration = 1.0f;

	UPROPERTY()
	bool RequiresPlayer = true;

	UPROPERTY()
	float Range = 400.0f;

	UPROPERTY()
	float Cost = 0.0f;

	UPROPERTY()
	bool UsesConstructionBuilding = false;

	UPROPERTY()
	bool CancelOnDamage = true;

	UPROPERTY()
	bool DestroyOnFail = false;

	UPROPERTY()
	bool RefundOnFail = true;
};

UCLASS()
class BASEDEFENSE_API ABuilding : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	void GiveMoney(float AnAmount, bool AToOwningPlayer, bool APenalty);
	virtual void Tick(float DeltaSeconds) override;
public:

	//void Initialise(EBuilding ABuilding);

	void Interact(FBuildingInteraction AnInteraction);

	void TickInteraction(float DeltaSeconds);


	void Construct(EBuilding ABuilding, class APlayerChar* AConstructor);

	void Upgrade(EBuildingUpgrade AnUpgrade, class APlayerChar* AnUpgrader);
	
	void CancelInteraction();
	void CompleteInteraction();

	virtual TArray<EGUICommand> GetCommands() override;

	void SetUpBuilding(EBuilding ABuilding);
	UFUNCTION()
	void OnMouseEnter(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
	void OnMouseLeave(UPrimitiveComponent * TouchedComponent);

	UFUNCTION()
	void OnAttacked(AActor* AttackingTarget, float ADamage);

	UFUNCTION()
	virtual bool RepairPressed() override;

	UFUNCTION()
	virtual bool UsePressed() override;

	/*UFUNCTION()
	virtual bool RepairReleased() override;*/

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION()
	void GenerateIncome();


	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnDamageText(const FString& AText, FColor AColor);

	void WhatDo();

	UFUNCTION()
	void OnRep_SetInteraction();
	UFUNCTION()
	void OnRep_SetMaxInteraction();

	UFUNCTION()
	void OnRep_SetFloatingHeight();

	void ApplyBuffs();

	void AddBuff(FBuildingBuffStruct ABuff);

	void AddBuffs(TArray<FBuildingBuffStruct> ABuffs);

	UFUNCTION()
	void RemoveExpiredBuffs();

	bool UpdateUpgradable();


public:
	EBuildingUpgrade CurrentUpgrade = EBuildingUpgrade::None;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere)
	EBuilding Building = EBuilding::None;

	UPROPERTY(VisibleAnywhere)
	FBuildingData BaseBuildingData;

	TArray<FBuildingBuffStruct> Buffs;

	UPROPERTY(VisibleAnywhere)
	FBuildingData BuffedBuildingData;


	UPROPERTY(Replicated)
	bool Upgradable = false;


	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComponent = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* FloatingWidget = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* OverlapCapsule = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UFloatingBuildingInfo* FloatingInfo = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* RangeCapsule = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	bool Constructing = false;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SetInteraction)
	float CurrentInteractionTime = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SetMaxInteraction)
	float MaxInteractionTime = 0;

	
	FBuildingInteraction CurrentInteraction;
	bool Interacting = false;

	UPROPERTY(VisibleAnywhere)
	TArray<class UBuildingAIAction*> Actions;
	UPROPERTY(VisibleAnywhere)
	class UBuildingAIAction* CurrentAction = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_SetFloatingHeight)
	float FloatingHeight = 0;

	bool BeingRepaired = false;

	TArray<EBuildingUpgrade> LevelUpgrades;
	TArray<EBuildingUpgrade> PregameUpgrades;

protected:
	class UBDGameInstance* GameInstance = nullptr;
	class ABDGameState* GameState = nullptr;
	class ABDPlayerController* Controller = nullptr;
	class APlayerChar* Character = nullptr;
	class APlayerChar* OwningCharacter = nullptr;

};
