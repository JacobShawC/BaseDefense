// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveRow.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API USaveRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetUp(FString ASaveName);
	void SetUp();
	
	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ButtonText;


	bool NewGame = false;

	FString SaveName = "";

private:
	class UBDGameInstance* Instance = nullptr;
};
