// Fill out your copyright notice in the Description page of Project Settings.

#include "Slot.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Engine/World.h"
#include "Controllers/BDPlayerController.h"
#include "Input/Reply.h"
#include "UObject/WeakObjectPtr.h"

void USlot::Hovered()
{
	if (ShouldAnimateHover && !HoverAnimationFoward)
	{
		if (HoverAnimationFinished)
		{
			HoverAnimationFinished = false;
		}

		FWidgetAnimationDynamicEvent AnimEvent;
		AnimEvent.BindUFunction(this, "SetBoolTest");
		BindToAnimationFinished(HoverAnimation, AnimEvent);

		float Time = GetAnimationCurrentTime(HoverAnimation);
		PlayAnimation(HoverAnimation, Time, 1, EUMGSequencePlayMode::Forward, 3.0f);
		HoverAnimationFoward = true;
	}
}

void USlot::UnHovered()
{
	if (ShouldAnimateHover && HoverAnimationFoward)
	{
		float Time;
		if (HoverAnimationFinished)
		{
			Time = 0.0f;
			HoverAnimationFinished = false;
		}
		else
		{
			Time = HoverAnimation->GetEndTime() - GetAnimationCurrentTime(HoverAnimation);
			//Time = GetAnimationCurrentTime(HoverAnimation);

		}
		//PlayAnimation(HoverAnimation, HoverAnimation->GetEndTime() - GetAnimationCurrentTime(HoverAnimation), 1, EUMGSequencePlayMode::Reverse, 1.0f);
		HoverAnimationFoward = false;
		PlayAnimation(HoverAnimation, Time, 1, EUMGSequencePlayMode::Reverse, 3.0f);
	}
}

void USlot::SetCooldownText(FString AString)
{
	if (Cooldown)
	{
		Cooldown->SetText(FText::FromString(AString));
	}
}

void USlot::SetPriceText(FString AString)
{
	if (Price)
	{
		Price->SetText(FText::FromString(AString));
	}
}

void USlot::SetImage(UTexture2D* AnImage)
{
	if (SlotImage)
	{
		SlotImage->SetBrushFromTexture(AnImage);
	}
}

void USlot::SetCooldownVisiblity(bool AHidden)
{
	if (Cooldown)
	{
		if (AHidden)
		{
			Cooldown->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Cooldown->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USlot::SetPriceVisiblity(bool AHidden)
{
	if (Price)
	{
		if (AHidden)
		{
			Price->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Price->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USlot::SetImageVisiblity(bool AHidden)
{
	if (SlotImage)
	{
		if (AHidden)
		{
			SlotImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SlotImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (AnimationsMap.Contains("HoverAnimationBP"))
	{
		HoverAnimation = *AnimationsMap.Find("HoverAnimationBP");
	}
}

void USlot::SetBoolTest()
{
	HoverAnimationFinished = true;
}

