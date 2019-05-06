// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructLibrary.h"
#include "PreInfoSlot.generated.h"
DECLARE_MULTICAST_DELEGATE_OneParam(FInfoSlotClickedDelegate, UPreInfoSlot*);

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UPreInfoSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetUp(ELevelDifficulty ADifficulty, FLevelData AData);
	void SetUp(EBuildingUpgrade AnUpgrade, FBuildingData AData);
	void OnButtonClicked();
	void Refresh();


public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotButtonText;	

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotTitle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotDescription;

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;

	bool LevelOrBuilding = true;

	FInfoSlotClickedDelegate OnSelfClicked;

	FBuildingData BuildingData;
	FLevelData LevelData;

	ELevelDifficulty LevelDifficulty;

protected:

};
