//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
//#include "StructLibrary.h"
//#include "LoadoutSelector.generated.h"
//
///**
// * 
// */
//UCLASS()
//class BASEDEFENSE_API ULoadoutSelector : public UUserWidget
//{
//	GENERATED_BODY()
//	
//public:
//	void Setup();
//
//	void SetupBuildings();
//
//	void SetupBuildingInformation(FBuildingData ABuildingData);
//
//	void RefreshBuildings();
//
//	void RefreshInformation();
//
//	void RefreshRewards();
//
//	void PreBuildingClicked(class UPreBuilding* ABuilding);
//
//	void PreInfoSlotClicked(class UPreInfoSlot* AnInfoSlot);
//
//private:
//	UFUNCTION()
//	void OnStartButtonClicked();
//
//
//
//protected:
//
//
//	UPROPERTY(meta = (BindWidget))
//		class UTextBlock* TotalLevelRewards = nullptr;
//
//	UPROPERTY(meta = (BindWidget))
//		class UTextBlock* RemainingLevelRewards = nullptr;
//
//	UPROPERTY(meta = (BindWidget))
//		class UButton* StartButton = nullptr;
//
//	UPROPERTY(meta = (BindWidget))
//		class UButton* ReadyButton = nullptr;
//
//	UPROPERTY(meta = (BindWidget))
//		class UUniformGridPanel* BuildingsGridPanel = nullptr;
//
//	UPROPERTY(meta = (BindWidget))
//		class UImage* InformationImage = nullptr;
//
//	UPROPERTY(meta = (BindWidget))
//		class UTextBlock* InformationTitle = nullptr;
//
//	UPROPERTY(meta = (BindWidget))
//		class UTextBlock* InformationText = nullptr;
//
//	UPROPERTY(meta = (BindWidget))
//		class UVerticalBox* InformationSlotBox = nullptr;
//
//	class UBDGameInstance* GameInstance = nullptr;
//	class UBDSaveGame* CurrentSave = nullptr;
//
//	int LevelRewards = 0;
//
//	//Selected level
//	EBuilding SelectedBuilding = EBuilding::None;
//
//};
