// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreLevel.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UPreLevel : public UUserWidget
{
	GENERATED_BODY()
	
	void SetUp(EBuilding ABuilding);

private:
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* ToggleButton = nullptr;
private:


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Description = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UnlockCost = nullptr;

};
