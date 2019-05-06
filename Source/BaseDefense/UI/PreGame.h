// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructLibrary.h"
#include "PreGame.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UPreGame : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void Setup();

	void RefreshLevels();
	
	void SetUpLevelInformation(FLevelData ALevelData);

	void RefreshInformation();

	void RefreshLevelRewards();

	void PreLevelClicked(class UPreLevel* ALevel);

	void PreInfoSlotClicked(class UPreInfoSlot* AnInfoSlot);

private:
	UFUNCTION()
	void OnStartButtonClicked();
	UFUNCTION()
	void OnInviteFriendsButtonClicked();
	UFUNCTION()
	void OnSettingsButtonClicked();
	UFUNCTION()
	void OnLeaveButtonClicked();


protected:
	
	UPROPERTY(meta = (BindWidget))
		class UButton* InviteFriendsButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* LeaveButton;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* LevelsGridPanel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InformationText;

	UPROPERTY(meta = (BindWidget))
		class UImage* InformationImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* InformationTitle;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedLevelText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedDifficultyText;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* InformationSlotBox;

	class UBDGameInstance* GameInstance = nullptr;
	class UBDSaveGame* CurrentSave = nullptr;

	int LevelRewards = 0;

	//Selected level
	ELevel SelectedLevel = ELevel::None;

	//Selected Build
	FLoadout* SelectedBuild = nullptr;
};
