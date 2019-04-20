// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructLibrary.h"
#include "PreBuilding.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UPreBuilding : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetUp(EBuilding ABuilding);

private:
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* LevelToggleButton = nullptr;
private:
	

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Image = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Description = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PurchaseCost = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UnlockCost = nullptr;


};
