// Fill out your copyright notice in the Description page of Project Settings.

#include "Command.h"

#include "Components/TextBlock.h"

void UCommand::SetText(FText AText)
{
	CommandText->SetText(AText);
}
