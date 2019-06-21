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
		class UTextBlock* SelectedDifficultyText;

	class UBDGameInstance* GameInstance = nullptr;
	class UBDSaveGame* CurrentSave = nullptr;


};
