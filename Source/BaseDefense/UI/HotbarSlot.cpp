// Fill out your copyright notice in the Description page of Project Settings.

#include "HotbarSlot.h"
#include "Main/BDGameInstance.h"
#include "UI/Hotbar.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
UHotbarSlot::UHotbarSlot(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

bool UHotbarSlot::SelectPressed()
{
	if (Hotbar)
	{
		Hotbar->Select(Index);
		return true;
	}
	return false;
}
bool UHotbarSlot::SelectReleased()
{
	return false;
}

void UHotbarSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	GetAnimations();

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
		FBuildingData* Data = GameInstance->Buildings.Find(ABuilding);
		Texture = Data->Thumbnail;
		//UTexture2D* Texture = BarrelImage.;
		if (Texture)
		{
			SlotImage->SetBrushFromTexture(Texture, false);
			SetImageVisiblity(true);
		}
	}
}

void UHotbarSlot::Initialise(UHotbar* AHotbar, EBuilding ABuilding, int AIndex)
{
	SetBuilding(ABuilding);
	Hotbar = AHotbar;
	Index = AIndex;
}
