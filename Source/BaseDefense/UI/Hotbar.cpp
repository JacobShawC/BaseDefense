// Fill out your copyright notice in the Description page of Project Settings.

#include "Hotbar.h"
#include "Components/UniformGridPanel.h"
#include "UI/HotbarSlot.h"
#include "UI/GUI.h"
#include "UObject/ConstructorHelpers.h"
#include "Main/BDGameInstance.h"
#include "Components/UniformGridSlot.h"
#include "Main/BDPlayerState.h"
#include "Public/TimerManager.h"
#include "Engine/World.h"
#include "PlayerChar.h"
#include "BDPlayerController.h"
UHotbar::UHotbar(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UHotbar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	PlayerState = Cast<ABDPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	for (int i = 0; i < SlotNum; i++)
	{
		UHotbarSlot* Widget = CreateWidget<UHotbarSlot>(this, GameInstance->Widgets.Find("HotbarSlot")->Get());
		Widget->Initialise(this, EBuilding::None, i);
		SlotPanel->AddChildToUniformGrid(Widget)->SetColumn(i);
		Slots.Add(Widget);

	}
	Refresh();
	HasInitialized = true;
	Select(CurrentSelected);
}


void UHotbar::Refresh()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		APawn* Pawn = nullptr;
		Pawn = Controller->GetPawn();
		if (Pawn)
		{
			PlayerState = Cast<ABDPlayerState>(Controller->GetPawn()->GetPlayerState());
		}
	}

	if (PlayerState)
	{
		PlayerState = Cast<ABDPlayerState>(Controller->PlayerState);
		for (int i = 0; i < PlayerState->PlayerData.HotbarSlots.Num(); i++)
		{
			if (Slots.Num() > i)
			{
				Slots[i]->SetBuilding(PlayerState->PlayerData.HotbarSlots[i]);
			}
		}
	}
	else
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UHotbar::Refresh, 0.1f, false, -1.0f);

	}
	
	return;
}



void UHotbar::Select(int Select)
{
	EBuilding SelectedBuilding = EBuilding::None;

	if (Select == -1)
	{
		if (CurrentSelected != -1 && Slots.Num() > Select && Slots[CurrentSelected]->IsValidLowLevel())
		{
			Slots[CurrentSelected]->UnHovered();
		}
		CurrentSelected = Select;
		SelectedBuilding = EBuilding::None;
	}
	else if (HasInitialized && Slots.Num() > Select && Slots[CurrentSelected]->IsValidLowLevel() && Slots[Select]->IsValidLowLevel())
	{
		Slots[CurrentSelected]->UnHovered();
		Slots[Select]->Hovered();
		CurrentSelected = Select;
		;

		SelectedBuilding = Slots[CurrentSelected]->Building;
	}


	ABDPlayerController* Controller = nullptr;
	Controller = Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController());

	if (Controller != nullptr)
	{
		Controller->SelectedBuilding = SelectedBuilding;
	}


	return;
}

void UHotbar::Scroll(bool UpDown)
{
	if (UpDown)
	{
		if (CurrentSelected > 0)
		{
			Select(CurrentSelected - 1);
			return;
		}
	}
	else
	{
		if (CurrentSelected < SlotNum - 1)
		{
			Select(CurrentSelected + 1);
			return;
		}
	}

	return;
}

EBuilding UHotbar::GetSelectedBuilding()
{
	if (CurrentSelected != -1 && Slots.Num() > CurrentSelected && Slots[CurrentSelected])
	{
		return Slots[CurrentSelected]->Building;
	}
	return EBuilding::None;
}
