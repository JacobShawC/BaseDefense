// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageText.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UDamageText::Initialise(FString AString, FColor AColor)
{
	SetText(AString);
	SetColor(AColor);
	Animation = *AnimationsMap.Find("Animation");
	if (Animation != nullptr)
	{
		PlayAnimation(Animation);
	}
}

void UDamageText::SetText(FString AString)
{
	DamageText->SetText(FText::FromString(AString));
}

void UDamageText::SetColor(FColor AColor)
{
	DamageText->SetColorAndOpacity(FSlateColor(FLinearColor(AColor)));
}

void UDamageText::AnimationFinished()
{
	//this->destroy
}
