// Fill out your copyright notice in the Description page of Project Settings.


#include "PreLevel.h"
#include "PreGame.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
void UPreLevel::SetUp(FLevelData ALevel, UPreGame* AnOwner)
{
	LevelData = ALevel;
	Owner = AnOwner;

	Name->SetText(FText::FromString(ALevel.Name));
	Image->SetBrushFromTexture(ALevel.Thumbnail);
	Description->SetText(FText::FromString(ALevel.Description));
	UnlockCost->SetText(FText::AsNumber(ALevel.PreGameUnlockCost));
}

void UPreLevel::Unlocked(bool AnUnlocked)
{

}

void UPreLevel::OnButtonClicked()
{
	if (Owner != nullptr)
	{
		Owner->PreLevelClicked(this);
	}
}