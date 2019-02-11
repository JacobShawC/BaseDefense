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

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION()
	void GenerateIncome();

	UFUNCTION()
	void SpawnIncomeText(FString AText);

	void WhatDo();

	UFUNCTION()
	void OnRep_SetConstruction();
	UFUNCTION()
	void OnRep_SetMaxConstruction();

	UFUNCTION()
	void OnRep_SetFloatingHeight();

public:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere)
	EBuilding Building = EBuilding::None;
	UPROPERTY(VisibleAnywhere)
	FBuildingData BuildingData;
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
	FBuildingData ConstructedBuildingData;

	UPROPERTY(VisibleAnywhere)
	class APlayerChar* Constructor = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<class UBuildingAIAction*> Actions;
	UPROPERTY(VisibleAnywhere)
	class UBuildingAIAction* CurrentAction = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_SetFloatingHeight)
	float FloatingHeight = 0;

protected:
	class UBDGameInstance* GameInstance = nullptr;
	class ABDGameState* GameState = nullptr;
	

};
