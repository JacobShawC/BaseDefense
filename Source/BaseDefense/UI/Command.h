// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BDUserWidget.h"
#include "Command.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UCommand : public UBDUserWidget
{
	GENERATED_BODY()
	

public:
	EGUICommand Command = EGUICommand::None;

	void SetText(FText AText);


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CommandText;
};
