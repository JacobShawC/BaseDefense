// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BDGameInstance.h"


#include "Main/StructLibrary.h"

#include "CoreMinimal.h"
#include "BDUserWidget.h"

#include "HotbarSlot.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UHotbarSlot : public UBDUserWidget
{
	GENERATED_BODY()
private:
	UHotbarSlot(const FObjectInitializer& ObjectInitializer);
public:
	void UnHovered();
	void ButtonClicked();
	void SetCooldownText(FString AString);
	void SetPriceText(FString AString);
	void SetImage(UTexture2D* AnImage);
	void SetCooldownVisiblity(bool AHidden);
	void SetPriceVisiblity(bool AHidden);
	void SetImageVisiblity(bool AHidden);
	void NativeOnInitialized() override;

	void SetBoolTest();
	void SetBuilding(EBuilding ABuilding);

	void Initialise(class UHotbar* AHotbar, EBuilding ABuilding, int AIndex);

	void Hovered();
	class UHotbar* Hotbar;

	EBuilding Building = EBuilding::None;
	int Index;

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
protected:
	class UBDGameInstance* GameInstance = nullptr;
};
