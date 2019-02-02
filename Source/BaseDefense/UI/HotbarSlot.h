// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Slot.h"
#include "Main/StructLibrary.h"

#include "HotbarSlot.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UHotbarSlot : public USlot
{
	GENERATED_BODY()
private:
	UHotbarSlot(const FObjectInitializer& ObjectInitializer);
public:
	virtual bool SelectPressed() override;
	virtual bool SelectReleased() override;
	void NativeOnInitialized() override;

	void SetBuilding(EBuilding ABuilding);

	void Initialise(class UHotbar* AHotbar, EBuilding ABuilding, int AIndex);


	class UHotbar* Hotbar;

	EBuilding Building = EBuilding::None;
	int Index;

protected:
	class UBDGameInstance* GameInstance = nullptr;
};
