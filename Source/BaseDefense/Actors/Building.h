// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction.h"
#include "StructLibrary.h"
#include "Building.generated.h"

UCLASS()
class BASEDEFENSE_API ABuilding : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	virtual void Tick(float DeltaSeconds) override;
public:

	//void Initialise(EBuilding ABuilding);
	void Construct(EBuilding ABuilding, class APlayerChar* AConstructor);
	void CancelConstruction();
	void SetUpBuilding(EBuilding ABuilding);
	UFUNCTION()
	void OnMouseEnter(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
	void OnMouseLeave(UPrimitiveComponent * TouchedComponent);

	UFUNCTION()
	virtual bool RepairPressed() override;

	/*UFUNCTION()
	virtual bool RepairReleased() override;*/

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION()
	void GenerateIncome();


	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnDamageText(const FString& AText, FColor AColor);

	void WhatDo();

	UFUNCTION()
	void OnRep_SetConstruction();
	UFUNCTION()
	void OnRep_SetMaxConstruction();

	UFUNCTION()
	void OnRep_SetFloatingHeight();

	void ApplyBuffs();

	void AddBuff(FBuildingBuffStruct ABuff);

	void AddBuffs(TArray<FBuildingBuffStruct> ABuffs);

	UFUNCTION()
	void RemoveExpiredBuffs();



public:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere)
	EBuilding Building = EBuilding::None;

	UPROPERTY(VisibleAnywhere)
	FBuildingData BaseBuildingData;

	TArray<FBuildingBuffStruct> Buffs;

	UPROPERTY(VisibleAnywhere)
	FBuildingData BuffedBuildingData;

	UPROPERTY(VisibleAnywhere)
	FBuildingData ConstructionBuildingData;


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

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SetConstruction)
	float CurrentConstructionTime = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SetMaxConstruction)
	float MaxConstructionTime = 0;

	

	UPROPERTY(VisibleAnywhere)
	class APlayerChar* Constructor = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<class UBuildingAIAction*> Actions;
	UPROPERTY(VisibleAnywhere)
	class UBuildingAIAction* CurrentAction = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_SetFloatingHeight)
	float FloatingHeight = 0;


	bool BeingRepaired = false;

protected:
	class UBDGameInstance* GameInstance = nullptr;
	class ABDGameState* GameState = nullptr;
	class ABDPlayerController* Controller = nullptr;
	class APlayerChar* Character = nullptr;
};
