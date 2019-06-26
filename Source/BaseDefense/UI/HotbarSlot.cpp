// Fill out your copyright notice in the Description page of Project Settings.

#include "HotbarSlot.h"
#include "Main/BDGameInstance.h"
#include "UI/Hotbar.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include <WidgetAnimation.h>
#include <Image.h>
#include <Button.h>
#include <UserWidget.h>
#include <TextBlock.h>

UHotbarSlot::UHotbarSlot(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UHotbarSlot::SetBuilding(EBuilding ABuilding)
{
	Building = ABuilding;
	if (ABuilding == EBuilding::None)
	{
		SetImageVisiblity(false);
		return;
	}
	if (GameInstance)
	{
		UTexture2D* Texture = nullptr;
		FBuildingData* Data = nullptr;
		Data = GameInstance->Buildings.Find(ABuilding);
		if (Data != nullptr)
		{
			Texture = Data->Thumbnail;
			//UTexture2D* Texture = BarrelImage.;
			if (Texture)
			{
				SlotImage->SetBrushFromTexture(Texture, false);
				SetImageVisiblity(true);
			}
			SetPriceText(FString::FromInt(Data->Cost));
			if (Data->Cost == 0)
			{
				SetPriceVisiblity(false);
			}
			else
			{
				SetPriceVisiblity(true);
			}
			
		}
		

	}
}

void UHotbarSlot::Initialise(UHotbar* AHotbar, EBuilding ABuilding, int AIndex)
{
	SetBuilding(ABuilding);
	Hotbar = AHotbar;
	Index = AIndex;
}

void UHotbarSlot::Hovered()
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

void UHotbarSlot::UnHovered()
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

void UHotbarSlot::ButtonClicked()
{
	Hotbar->Select(Index);
}

void UHotbarSlot::SetCooldownText(FString AString)
{
	if (Cooldown)
	{
		Cooldown->SetText(FText::FromString(AString));
	}
}

void UHotbarSlot::SetPriceText(FString AString)
{
	if (Price)
	{
		Price->SetText(FText::FromString(AString));
	}
}

void UHotbarSlot::SetImage(UTexture2D* AnImage)
{
	if (SlotImage)
	{
		SlotImage->SetBrushFromTexture(AnImage);
	}
}

void UHotbarSlot::SetCooldownVisiblity(bool AHidden)
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

void UHotbarSlot::SetPriceVisiblity(bool AHidden)
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

void UHotbarSlot::SetImageVisiblity(bool AHidden)
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

void UHotbarSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	FScriptDelegate ButtonDel;
	ButtonDel.BindUFunction(this, "ButtonClicked");

	SlotButton->OnClicked.AddUnique(ButtonDel);
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	GetAnimations();

	if (AnimationsMap.Contains("HoverAnimationBP"))
	{
		HoverAnimation = *AnimationsMap.Find("HoverAnimationBP");
	}
}

void UHotbarSlot::SetBoolTest()
{
	HoverAnimationFinished = true;
}

