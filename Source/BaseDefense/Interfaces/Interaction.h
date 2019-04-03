// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StructLibrary.h"
#include "Interaction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEDEFENSE_API IInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual bool SelectPressed();

	UFUNCTION()
	virtual bool SelectReleased();

	UFUNCTION()
	virtual bool SelectAltPressed();

	UFUNCTION()
	virtual bool SelectAltReleased();
	
	UFUNCTION()
	virtual bool UsePressed();
	
	UFUNCTION()
	virtual bool UseReleased();

	UFUNCTION()
	virtual bool UseAltPressed();
	
	UFUNCTION()
	virtual bool UseAltReleased();

	UFUNCTION()
	virtual bool RepairPressed();
	
	UFUNCTION()
	virtual bool RepairReleased();

	UFUNCTION()
	virtual TArray<EGUICommand> GetCommands();
};
