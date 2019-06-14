// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Main/StructLibrary.h"
#include "BDPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FPlayerStateVariableUpdated);
DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerStateVariableOneParamUpdated, float);

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

	UFUNCTION()
	void OnRep_RemainingLevelRewards();
	UFUNCTION()
	void OnRep_Loadout();

	void SelectBuildingUpgrade(EBuilding ABuilding, EBuildingUpgrade AnUpgrade, bool AddOrRemove);

	void RefreshRemainingLevelRewards();

	void BeginPlay() override;

private:
public:

	FPlayerStateVariableUpdated RemainingLevelRewardsUpdated;
	FPlayerStateVariableUpdated LoadoutUpdated;
	FPlayerStateVariableOneParamUpdated MoneyUpdated;
	
	UPROPERTY(Replicated)
	FPlayerData PlayerData;

	UPROPERTY(ReplicatedUsing = OnRep_Money)
	float Money = 0;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingLevelRewards)	int RemainingLevelRewards = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Loadout)	FLoadout Loadout;

};
