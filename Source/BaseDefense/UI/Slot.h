// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BDUserWidget.h"
#include "Interfaces/Interaction.h"
#include "Slot.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API USlot : public UBDUserWidget
{
	GENERATED_BODY()
	
public:

	//USlot(const FObjectInitializer & ObjectInitializer);
	void Hovered();
	void UnHovered();
	void ButtonClicked();

	void SetCooldownText(FString AString);
	void SetPriceText(FString AString);
	void SetImage(class UTexture2D* AnImage);

	void SetCooldownVisiblity(bool AHidden);
	void SetPriceVisiblity(bool AHidden);
	void SetImageVisiblity(bool AHidden);

	void NativeOnInitialized() override;


protected:

	UFUNCTION()
	void SetBoolTest();
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Price;

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Cooldown;

	UPROPERTY(meta = (BindWidget))
	class UImage* SlotImage;

	UPROPERTY(BlueprintReadWrite)
	class UWidgetAnimation* HoverAnimation;

	float AnimationTime;

	UPROPERTY(BlueprintReadWrite)
	bool HoverAnimationFinished = false;
	bool HoverAnimationFoward = false;

	bool ShouldAnimateHover = true;


	int Clicked = 0;
};
