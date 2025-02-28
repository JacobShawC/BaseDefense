// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BDSphereComponent.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UBDSphereComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	uint32 ID;
	TArray<USphereComponent*> OverlappingSpheres;
};
