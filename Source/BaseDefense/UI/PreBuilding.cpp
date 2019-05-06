// Fill out your copyright notice in the Description page of Project Settings.


#include "PreBuilding.h"
#include "PreGame.h"
#include "LoadoutSelector.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UPreBuilding::SetUp(FBuildingData ABuilding)
{
	BuildingData = ABuilding;
	Name->SetText(FText::FromString(ABuilding.Name));
	Image->SetBrushFromTexture(ABuilding.Thumbnail);
	Description->SetText(FText::FromString(ABuilding.Description));
	UnlockCost->SetText(FText::AsNumber(ABuilding.PreGameUnlockCost));
	PurchaseCost->SetText(FText::AsNumber(ABuilding.PreGameCost));

	BuildingButton->OnClicked.AddDynamic(this, &UPreBuilding::OnButtonClicked);

}


void UPreBuilding::OnButtonClicked()
{
	OnSelfClicked.Broadcast(this);
}

void UPreBuilding::SetLocked(bool LockedOrUnlocked)
{
	if (LockedOrUnlocked)
	{
		SetRenderOpacity(0.5f);
	}
	else
	{
		SetRenderOpacity(1.0f);
	}
}
