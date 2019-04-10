// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Main/StructLibrary.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:


	void RefreshFriends();

	void RefreshSaves();

public:
	void SetUp();

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	class USlider* VolumeSlider;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* RequiresInviteCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* MainPanel;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* FriendsList;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SaveList;

	TSubclassOf<UUserWidget>* FriendRowWidgetClass = nullptr;
	TSubclassOf<UUserWidget>* SaveRowWidgetClass = nullptr;

	TArray<class UFriendRow> FriendWidgets;
	TArray<class USaveRow> SaveWidgets;

	class UBDGameInstance* Instance = nullptr;
};
