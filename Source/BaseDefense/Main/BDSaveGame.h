// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BDSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UBDSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UBDSaveGame();


	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString UserSaveName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int Points = 0;

};
