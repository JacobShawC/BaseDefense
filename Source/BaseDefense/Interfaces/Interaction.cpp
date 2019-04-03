// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction.h"

// Add default functionality here for any IInteraction functions that are not pure virtual.
bool IInteraction::SelectPressed()
{
	return false;
}

bool IInteraction::SelectReleased()
{
	return false;
}

bool IInteraction::SelectAltPressed()
{
	return false;
}

bool IInteraction::SelectAltReleased()
{
	return false;
}

bool IInteraction::UsePressed()
{
	return false;
}

bool IInteraction::UseReleased()
{
	return false;
}

bool IInteraction::UseAltPressed()
{
	return false;
}

bool IInteraction::UseAltReleased()
{
	return false;
}

bool IInteraction::RepairPressed()
{
	return false;
}

bool IInteraction::RepairReleased()
{
	return false;
}

TArray<EGUICommand> IInteraction::GetCommands()
{
	return TArray<EGUICommand>();
}
