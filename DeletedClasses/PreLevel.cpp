// Fill out your copyright notice in the Description page of Project Settings.


#include "PreLevel.h"
#include "PreGame.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2D.h"
void UPreLevel::SetUp(FLevelData ALevel)
{
	LevelData = ALevel;

	Name->SetText(FText::FromString(ALevel.Name));
	if (ALevel.MiniMap != nullptr)
	{
		Image->SetBrushFromTexture(ALevel.MiniMap);
	}
	Description->SetText(FText::FromString(ALevel.Description));
	UnlockCost->SetText(FText::AsNumber(ALevel.PreGameUnlockCost));

	LevelButton->OnClicked.AddDynamic(this, &UPreLevel::OnButtonClicked);

}

void UPreLevel::SetLocked(bool LockedOrUnlocked)
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

void UPreLevel::OnButtonClicked()
{
	OnSelfClicked.Broadcast(this);
}