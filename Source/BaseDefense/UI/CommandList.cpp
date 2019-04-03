// Fill out your copyright notice in the Description page of Project Settings.

#include "CommandList.h"
#include "Command.h"
#include "Engine/World.h"
#include "BDPlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Containers/UnrealString.h"
#include "Components/VerticalBox.h"
#include "BDGameInstance.h"
#include "Kismet/KismetInputLibrary.h"
void UCommandList::SetCommandList(TArray<EGUICommand> ACommands)
{
	TArray<EGUICommand> CommandsToAdd;
	TArray<EGUICommand> CommandsToRemove;
	for (EGUICommand ACommand : ACommands)
	{
		if (!CommandMap.Contains(ACommand))
		{
			CommandsToAdd.Add(ACommand);
		}
	}

	for (const TPair<EGUICommand, class UCommand*>& CommandPair : CommandMap)
	{
		if (!ACommands.Contains(CommandPair.Key))
		{
			CommandsToRemove.Add(CommandPair.Key);
		}
	}

	for (EGUICommand ACommand : CommandsToAdd)
	{
		AddCommand(ACommand);
	}

	for (EGUICommand ACommand : CommandsToRemove)
	{
		RemoveCommand(ACommand);
	}
}


FString UCommandList::GetTextFromCommand(EGUICommand ACommand)
{

	FString ActionName = "";
	FString HotkeyString = "";
	FString CommandString = "";
	FString ReturnString = "";
	switch (ACommand)
	{
		case EGUICommand::Construct: ActionName = "Select"; CommandString = "Construct"; break;
		case EGUICommand::Destroy : ActionName = "UseAlt"; CommandString = "Destroy"; break;
		case EGUICommand::Equip : ActionName = "Use"; CommandString = "Equip"; break;
		case EGUICommand::Repair : ActionName = "Repair"; CommandString = "Repair"; break;
		case EGUICommand::Sell : ActionName = "UseAlt"; CommandString = "Sell"; break;
		case EGUICommand::Upgrade : ActionName = "Use"; CommandString = "Upgrade"; break;
	}


	
	if (Controller == nullptr)
	{
		Controller = Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController());
		if (Controller == nullptr) return ReturnString;
	}

	TArray<FInputActionKeyMapping> Bindings;
	Controller->GetKeysForAction(FName(*ActionName), Bindings);


	if (Bindings.Num() > 0)
	{
		HotkeyString = UKismetInputLibrary::Key_GetDisplayName(Bindings[0].Key).ToString();
	}

	ReturnString = HotkeyString + ": " + CommandString;

	return ReturnString;
}

void UCommandList::AddCommand(EGUICommand ACommand)
{
	//UHotbarSlot* Widget = CreateWidget<UHotbarSlot>(this, GameInstance->Widgets.Find("HotbarSlot")->Get());
	if (GameInstance == nullptr)
	{
		GameInstance = Cast<UBDGameInstance>(GetGameInstance());

		if (GameInstance == nullptr)
		{
			return;
		}
	}

	UCommand* Widget = CreateWidget<UCommand>(this, GameInstance->Widgets.Find("Command")->Get());

	FString CommandString = GetTextFromCommand(ACommand);

	Widget->SetText(FText::FromString(CommandString));
	CommandMap.Add(ACommand, Widget);
	Commands->AddChildToVerticalBox(Widget);
}

void UCommandList::RemoveCommand(EGUICommand ACommand)
{
	Cast<UCommand>(*CommandMap.Find(ACommand))->RemoveFromParent();
	CommandMap.Remove(ACommand);
}
