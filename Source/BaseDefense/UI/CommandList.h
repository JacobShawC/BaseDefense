// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructLibrary.h"
#include "Containers/UnrealString.h"

#include "CommandList.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UCommandList : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCommandList(TArray<EGUICommand> ACommands);

	FString GetTextFromCommand(EGUICommand ACommand);

	void AddCommand(EGUICommand ACommand);
	void RemoveCommand(EGUICommand ACommand);

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* Commands = nullptr;

	TMap<EGUICommand, class UCommand*> CommandMap;

private:
	class ABDPlayerController* Controller = nullptr;
	class UBDGameInstance* GameInstance = nullptr;
};
