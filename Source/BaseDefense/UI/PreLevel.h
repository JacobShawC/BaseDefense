// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructLibrary.h"
#include "PreLevel.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UPreLevel : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetUp(FLevelData ALevel, class UPreGame* AnOwner);
	void Unlocked(bool AnUnlocked);
	void OnButtonClicked();
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* LevelButton = nullptr;
private:
	class UPreGame* Owner;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Description = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UnlockCost = nullptr;


public:
	FLevelData LevelData;

};
