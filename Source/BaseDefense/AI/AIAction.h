// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "StructLibrary.h"
#include "AIAction.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UAIAction : public UObject
{
	GENERATED_BODY()
public:

	/** Activate action. */
	virtual void Activate();

	/** Should we activate action this time ? */
	virtual void SetShouldActivate();

	/** Abort action to start something else. */
	virtual void Abort();

	/** Can we abort this action? */
	virtual bool IsSafeToAbort() const;

	UPROPERTY()
	bool SafeToAbort = true;

	UPROPERTY()
	bool Executing = false;

	UPROPERTY()
	bool ShouldActivate = false;
};
