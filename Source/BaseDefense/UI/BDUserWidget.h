// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interaction.h"

#include "BDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UBDUserWidget : public UUserWidget, public IInteraction
{
	GENERATED_BODY()
public:
	/*virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;*/
	/*virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);*/
	void NativeOnInitialized() override;

	void GetAnimations();

public:
	TMap<FName, class UWidgetAnimation*> AnimationsMap;
};