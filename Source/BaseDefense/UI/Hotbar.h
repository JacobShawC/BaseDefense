// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Main/StructLibrary.h"
#include "Hotbar.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UHotbar : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UHotbar(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;

public:

	void Refresh();

	void Select(int Select);

	void Scroll(bool UpDown);

	EBuilding GetSelectedBuilding();
public:
	TArray<class UHotbarSlot*> Slots;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SlotPanel;


	int SlotNum = 10;
	int CurrentSelected = 0;

	bool HasInitialized = false;


protected:
	class UBDGameInstance* GameInstance = nullptr;
	class ABDPlayerState* PlayerState = nullptr;


};
