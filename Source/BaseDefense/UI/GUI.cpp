// Fill out your copyright notice in the Description page of Project Settings.

#include "GUI.h"
#include "Components/TextBlock.h"

UGUI::UGUI(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void UGUI::SetMoneyText(float AMoney)
{
	Money->SetText(FText::FromString(FString::SanitizeFloat(AMoney)));
}
