// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingInfo.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UFloatingInfo : public UUserWidget
{
	GENERATED_BODY()
public:

	void Initialise(FString AName, float AMaxHealth);
	void SetHealthText(float AHealth);
	void SetHealth(float AHealth);
	void SetMaxHealth(float AMaxHealth);
	void SetName(FString AHealth);
	void SetHealthProgressBar(float AHealth);
	void NativeOnInitialized() override;
	void SetHealthVisibility(bool AVisiblity);
	void SetHealthTextVisibility(bool AVisiblity);
	void SetHealthProgressBarVisibility(bool AVisiblity);
	void SetNameVisibility(bool AVisiblity);

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* HealthSizeBox;



	float MaxHealth = 100;
	float Health = 100;
};
