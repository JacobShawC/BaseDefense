// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructLibrary.h"
#include "GUI.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UGUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UGUI(const FObjectInitializer& ObjectInitializer);

public:
	void SetMoneyText(float AMoney);

	void SetCommandList(TArray<EGUICommand> ACommands);
/*
	UPROPERTY(meta = (BindWidget))
	class USlot* ATestSlot;*/
	UPROPERTY(meta = (BindWidget))
	class UHotbar* HotBar;

	UPROPERTY(meta = (BindWidget))
	class UCommandList* CommandList = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Money;

	UPROPERTY(meta = (BindWidget))
	class UMiniMap* MiniMap = nullptr;
};
