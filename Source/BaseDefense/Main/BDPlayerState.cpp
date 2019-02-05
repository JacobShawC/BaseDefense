// Fill out your copyright notice in the Description page of Project Settings.

#include "BDPlayerState.h"
#include "UnrealNetwork.h"
#include "GUI.h"
#include "BDPlayerController.h"
void ABDPlayerState::SetUpTestData()
{
	PlayerData.HotbarSlots.Add(EBuilding::Wall);

	PlayerData.HotbarSlots.Add(EBuilding::ArrowTower);

	PlayerData.HotbarSlots.Add(EBuilding::Farm);
}

void ABDPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABDPlayerState, PlayerData);
	DOREPLIFETIME(ABDPlayerState, Money);
}

void ABDPlayerState::OnRep_Money()
{
	ABDPlayerController* OwnerController = nullptr;
	OwnerController = Cast<ABDPlayerController>(GetOwner());
	if (OwnerController != nullptr)
	{
		if (OwnerController->GUIWidget != nullptr)
		{
			OwnerController->GUIWidget->SetMoneyText(Money);
		}
	}
}
