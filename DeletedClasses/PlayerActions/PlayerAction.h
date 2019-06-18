// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "StructLibrary.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "PlayerAction.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UPlayerAction : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void DoAction();
	virtual void CancelAction();
	
};
