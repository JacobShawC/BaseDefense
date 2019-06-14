// Fill out your copyright notice in the Description page of Project Settings.

#include "GUI.h"
#include "Components/TextBlock.h"
#include "BDPlayerState.h"
#include "CommandList.h"
#include "BDGameInstance.h"

UGUI::UGUI(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void UGUI::SetMoneyText(float AMoney)
{
	Money->SetText(FText::FromString(FString::SanitizeFloat(AMoney)));
}

void UGUI::SetCommandList(TArray<EGUICommand> ACommands)
{
	if (CommandList != nullptr)
	{
		CommandList->SetCommandList(ACommands);
	}
}

void UGUI::Setup()
{
	ABDPlayerState* PlayerState = GetOwningPlayerState<ABDPlayerState>();
	if (PlayerState != nullptr)
	{
		SetMoneyText(PlayerState->Money);
		PlayerState->MoneyUpdated.AddUObject(this, &UGUI::OnMoneyUpdated);
	}
}

void UGUI::OnMoneyUpdated(float AMoney)
{
	SetMoneyText(AMoney);
}
