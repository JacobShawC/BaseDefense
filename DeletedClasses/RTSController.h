// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSController.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API ARTSController : public APlayerController
{
	GENERATED_BODY()

protected:

	ARTSController(const FObjectInitializer& ObjectInitializer);
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
public:
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerUpdatePositions(FVector APawnPosition, FVector AMousePosition);

};
