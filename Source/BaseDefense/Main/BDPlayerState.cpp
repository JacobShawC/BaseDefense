// Fill out your copyright notice in the Description page of Project Settings.

#include "BDPlayerState.h"
#include "UnrealNetwork.h"
#include "GUI.h"
#include "BDPlayerController.h"
void ABDPlayerState::SetUpTestData()
{
	PlayerData.HotbarSlots.Add(EBuilding::Wall);

	PlayerData.HotbarSlots.Add(EBuilding::CannonTower);

	PlayerData.HotbarSlots.Add(EBuilding::Farm);
}

bool ABDPlayerState::ChangePlayerMoney(float AMoney)
{
		if (CanChangePlayerMoney(AMoney))
		{
			Money += AMoney;
			OnRep_Money();
			return true;
		}
		return false;
}

bool ABDPlayerState::CanChangePlayerMoney(float AMoney)
{
	if (Money + AMoney > 0)
	{
		return true;
	}
	return false;
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
