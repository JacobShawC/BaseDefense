// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BDUserWidget.h"
#include "Math/Color.h"
#include "DamageText.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UDamageText : public UBDUserWidget
{
	GENERATED_BODY()
	

public:
	//UDamageText(const FObjectInitializer& ObjectInitializer);


	void Initialise(FString AString, FColor AColor);
	void SetText(FString AString);
	void SetColor(FColor AColor);

	UFUNCTION()
	void AnimationFinished();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;
	class UWidgetAnimation* Animation = nullptr;
};
