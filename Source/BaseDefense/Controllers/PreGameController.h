// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PreGameController.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API APreGameController : public APlayerController
{
	GENERATED_BODY()
	

	APreGameController(const FObjectInitializer& ObjectInitializer);
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

public:
	class UPreGame* PreGame = nullptr;

private:
	class UBDGameInstance* GameInstance = nullptr;

};
