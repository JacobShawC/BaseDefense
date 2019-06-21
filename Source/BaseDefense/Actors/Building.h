// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructLibrary.h"
#include "Interaction.h"
#include <GameFramework/Actor.h>
#include "Building.generated.h"

UENUM()
enum class FBuildingStatus : uint8
{
	None 			UMETA(DisplayName = "None"),
	Constructing	UMETA(DisplayName = "Constructing"),
	Powered			UMETA(DisplayName = "Powered"),
	Unpowered		UMETA(DisplayName = "Unpowered"),
	Broken			UMETA(DisplayName = "Broken"),
	
};


UCLASS()
class BASEDEFENSE_API ABuilding : public AActor, public IInteraction
{
	GENERATED_BODY()



public:
	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UFloatingBuildingInfo* FloatingInfo = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Hitbox = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* RangeCapsule = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent = nullptr;

	FBuildingStatus BuildingStatus = FBuildingStatus::None;

	UPROPERTY(VisibleAnywhere)
	FBuildingData BuildingData;

	void WhatDo();

	UPROPERTY(VisibleAnywhere)
	class UBuildingAIAction* CurrentAction = nullptr;

};