// Fill out your copyright notice in the Description page of Project Settings.

#include "BDPlayerState.h"
#include "UnrealNetwork.h"

void ABDPlayerState::SetUpTestData()
{
	PlayerData.HotbarSlots.Add(EBuilding::ArrowTower);

	PlayerData.HotbarSlots.Add(EBuilding::Wall);
}

void ABDPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABDPlayerState, PlayerData);
}
