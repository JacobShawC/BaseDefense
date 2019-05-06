// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructLibrary.h"
#include "PreBuilding.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FButtonClickedDelegate, UPreBuilding*);


/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UPreBuilding : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetUp(FBuildingData ABuilding);
	void Unlocked(bool AnUnlocked);
	void OnButtonClicked();

	void Refresh();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BuildingButton = nullptr;
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

public:
	FBuildingData BuildingData;
	FButtonClickedDelegate OnSelfClicked;
};