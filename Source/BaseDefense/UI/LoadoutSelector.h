// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructLibrary.h"
#include "LoadoutSelector.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API ULoadoutSelector : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup();

	void SetupBuildings();

	void SetupBuildingInformation(FBuildingData ABuildingData);

	void RefreshBuildings();

	void RefreshInformation();

	void PreBuildingClicked(class UPreBuilding* ABuilding);

	void PreInfoSlotClicked(class UPreInfoSlot* AnInfoSlot);

private:
	UFUNCTION()
	void OnStartButtonClicked();



protected:


	

	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ReadyButton;

	UPROPERTY(meta = (BindWidget))
		class UUniformGridPanel* BuildingsGridPanel;

	UPROPERTY(meta = (BindWidget))
		class UImage* InformationImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* InformationTitle;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* InformationText;

	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* InformationSlotBox;

	class UBDGameInstance* GameInstance = nullptr;
	class UBDSaveGame* CurrentSave = nullptr;

	int LevelRewards = 0;

	//Selected level
	EBuilding SelectedBuilding = EBuilding::None;

};
