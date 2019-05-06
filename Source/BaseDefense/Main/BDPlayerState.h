// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Main/StructLibrary.h"
#include "BDPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FPlayerStateVariableUpdated);

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API ABDPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	void SetUpTestData();

	bool ChangePlayerMoney(float AMoney);
	bool CanChangePlayerMoney(float AMoney);
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	UFUNCTION()
	void OnRep_Money();

private:
	void OnRep_RemainingLevelRewards();
	void OnRep_Loadout();
public:

	FPlayerStateVariableUpdated RemainingLevelRewardsUpdated;
	FPlayerStateVariableUpdated LoadoutUpdated;
	
	UPROPERTY(Replicated)
	FPlayerData PlayerData;

	UPROPERTY(ReplicatedUsing = OnRep_Money)
	float Money = 500;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingLevelRewards)	int RemainingLevelRewards = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Loadout)	FLoadout Loadout;

};
