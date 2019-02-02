// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingBuildingInfo.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"

void UFloatingBuildingInfo::SetMaxConstruction(float AMaxConstruction)
{
	MaxConstruction = AMaxConstruction;
}

void UFloatingBuildingInfo::SetConstruction(float AConstruction)
{
	Construction = AConstruction;
	SetConstructionText(Construction);
	SetConstructionProgressBar(Construction);
}

void UFloatingBuildingInfo::SetConstructionText(float AConstruction)
{
	Construction = AConstruction;
	//FString ConstructionString = FString::FromInt(FMath::FloorToFloat(Construction)) + "/" + FString::FromInt(MaxConstruction);
	FString ConstructionString = FString::SanitizeFloat(Construction) + "/" + FString::FromInt(MaxConstruction);
	if (ConstructionText)
	{
		ConstructionText->SetText(FText::FromString(ConstructionString));
	}
}

void UFloatingBuildingInfo::SetConstructionProgressBar(float AConstruction)
{
	float Percent = Construction / MaxConstruction;
	if (ConstructionProgressBar)
	{
		ConstructionProgressBar->SetPercent(Percent);
	}
}

void UFloatingBuildingInfo::SetConstructionVisibility(bool AVisiblity)
{
	if(AVisiblity)
	{
		ConstructionSizeBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		ConstructionSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFloatingBuildingInfo::SetConstructionTextVisibility(bool AVisiblity)
{
	if (AVisiblity)
	{
		ConstructionText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		ConstructionText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFloatingBuildingInfo::SetConstructionProgressBarVisibility(bool AVisiblity)
{
	if (AVisiblity)
	{
		ConstructionProgressBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		ConstructionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}
