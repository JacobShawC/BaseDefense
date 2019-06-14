// Fill out your copyright notice in the Description page of Project Settings.

#include "BDPlayerState.h"
#include "UnrealNetwork.h"
#include "GUI.h"
#include "BDGameInstance.h"
#include "BDGameState.h"
#include "BDPlayerController.h"
//#include "ISimplexNoise.h"
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
	DOREPLIFETIME(ABDPlayerState, RemainingLevelRewards);
	DOREPLIFETIME(ABDPlayerState, Loadout);
}

void ABDPlayerState::OnRep_Money()
{
	ABDPlayerController* OwnerController = nullptr;

	MoneyUpdated.Broadcast(Money);
}

void ABDPlayerState::SelectBuildingUpgrade(EBuilding ABuilding, EBuildingUpgrade AnUpgrade, bool AddOrRemove)
{
	if (Role != ROLE_Authority) return;

	UBDGameInstance* GameInstance = Cast<UBDGameInstance>(GetGameInstance());


	TArray<EBuildingUpgrade> UpgradesToAdd;
	TArray<EBuildingUpgrade> UpgradesToRemove;
	TArray<EBuildingUpgrade> CheckCostOf;

	int Cost = 0;

	if (GameInstance != nullptr)
	{
		FBuildingData BuildingData = *(GameInstance->Buildings.Find(ABuilding));

		if (AddOrRemove)
		{
			//First we will work out the cost of the upgrade
			//Check player hasn't already upgraded/owned


			//check the upgrade isn't already added
			if (!(Loadout.Buildings.Contains(ABuilding) && Loadout.Buildings.Find(ABuilding)->Upgrades.Contains(AnUpgrade)))
			{
				UpgradesToAdd.Add(AnUpgrade);
				FPreBuildingData Data;
				if (Loadout.Buildings.Contains(ABuilding))
				{
					Data = *Loadout.Buildings.Find(ABuilding);
				}
				//Add up all necessary Upgrades that we need to add 
				if (AnUpgrade == EBuildingUpgrade::PreLevel1 || AnUpgrade == EBuildingUpgrade::PreLevel2 || AnUpgrade == EBuildingUpgrade::PreLevel3 || AnUpgrade == EBuildingUpgrade::PreLevel4)
				{
					CheckCostOf.Add(EBuildingUpgrade::PreLevel1);
				}
				if (AnUpgrade == EBuildingUpgrade::PreLevel2 || AnUpgrade == EBuildingUpgrade::PreLevel3 || AnUpgrade == EBuildingUpgrade::PreLevel4)
				{
					CheckCostOf.Add(EBuildingUpgrade::PreLevel2);
					UpgradesToRemove.Add(EBuildingUpgrade::PreLevel1);
				}
				if (AnUpgrade == EBuildingUpgrade::PreLevel3 || AnUpgrade == EBuildingUpgrade::PreLevel4)
				{
					CheckCostOf.Add(EBuildingUpgrade::PreLevel3);
					UpgradesToRemove.Add(EBuildingUpgrade::PreLevel2);

				}
				if (AnUpgrade == EBuildingUpgrade::PreLevel4)
				{
					CheckCostOf.Add(EBuildingUpgrade::PreLevel4);
					UpgradesToRemove.Add(EBuildingUpgrade::PreLevel3);
				}

				//go through radio button options and remove cost of other radio button and then add the selected one
				if (AnUpgrade == EBuildingUpgrade::PreChoice1 || AnUpgrade == EBuildingUpgrade::PreChoice2 || AnUpgrade == EBuildingUpgrade::PreChoice3)
				{
					CheckCostOf.Add(AnUpgrade);

					if (Data.Upgrades.Contains(EBuildingUpgrade::PreChoice1) && AnUpgrade != EBuildingUpgrade::PreChoice1)
					{
						FBuildingUpgrade TempUpgrade = *(BuildingData.LoadoutUpgrades.Find(EBuildingUpgrade::PreChoice1));
						Cost -= TempUpgrade.Cost;

						UpgradesToRemove.Add(EBuildingUpgrade::PreChoice1);
					}
					else if (Data.Upgrades.Contains(EBuildingUpgrade::PreChoice2) && AnUpgrade != EBuildingUpgrade::PreChoice2)
					{
						FBuildingUpgrade TempUpgrade = *(BuildingData.LoadoutUpgrades.Find(EBuildingUpgrade::PreChoice2));
						Cost -= TempUpgrade.Cost;
						UpgradesToRemove.Add(EBuildingUpgrade::PreChoice2);
					}
					else if (Data.Upgrades.Contains(EBuildingUpgrade::PreChoice3) && AnUpgrade != EBuildingUpgrade::PreChoice3)
					{
						FBuildingUpgrade TempUpgrade = *(BuildingData.LoadoutUpgrades.Find(EBuildingUpgrade::PreChoice3));
						Cost -= TempUpgrade.Cost;
						UpgradesToRemove.Add(EBuildingUpgrade::PreChoice3);
					}
				}


				//find out the costs of all those upgrades
				for (auto TempUp : CheckCostOf)
				{
					FBuildingUpgrade TempUpgrade = *(BuildingData.LoadoutUpgrades.Find(TempUp));
					Cost += TempUpgrade.Cost;
				}
			}
		}
		else
		{
			if (Loadout.Buildings.Contains(ABuilding) && Loadout.Buildings.Find(ABuilding)->Upgrades.Contains(AnUpgrade))
			{
				UpgradesToRemove.Add(AnUpgrade);
				FPreBuildingData Data;
				if (Loadout.Buildings.Contains(ABuilding))
				{
					Data = *Loadout.Buildings.Find(ABuilding);
				}
				TArray<EBuildingUpgrade> UpgradesCheck;

				//Add the initial removed upgrade to the list of refunded upgrades
				UpgradesCheck.Add(AnUpgrade);

				//Here we go through the multiple level upgrades to get the additional upgrades that need to be refunded.
				if (AnUpgrade == EBuildingUpgrade::PreLevel1 || AnUpgrade == EBuildingUpgrade::PreLevel2 || AnUpgrade == EBuildingUpgrade::PreLevel3)
				{
					UpgradesCheck.Add(EBuildingUpgrade::PreLevel4);
				}
				if (AnUpgrade == EBuildingUpgrade::PreLevel1 || AnUpgrade == EBuildingUpgrade::PreLevel2)
				{
					UpgradesCheck.Add(EBuildingUpgrade::PreLevel3);
				}
				if (AnUpgrade == EBuildingUpgrade::PreLevel1)
				{
					UpgradesCheck.Add(EBuildingUpgrade::PreLevel2);
				}

				//Add the previous option
				switch (AnUpgrade) {
				case EBuildingUpgrade::PreLevel4: UpgradesToAdd.Add(EBuildingUpgrade::PreLevel3);
					break;
				case EBuildingUpgrade::PreLevel3: UpgradesToAdd.Add(EBuildingUpgrade::PreLevel2);
					break;
				case EBuildingUpgrade::PreLevel2: UpgradesToAdd.Add(EBuildingUpgrade::PreLevel1);
					break;

				}
				
				//find out the refunded cost for the upgrades
				for (auto TempUp : UpgradesCheck)
				{
					if (Data.Upgrades.Contains(TempUp))
					{
						FBuildingUpgrade TempUpgrade = *(BuildingData.LoadoutUpgrades.Find(TempUp));
						Cost -= TempUpgrade.Cost;
					}
				}
			}

		}

		//if the player can pay the cost then add and remove the necessary upgrades
		if (RemainingLevelRewards - Cost >= 0)
		{
			RemainingLevelRewards -= Cost;

			FPreBuildingData PreBuildingData;
			if (Loadout.Buildings.Contains(ABuilding))
			{
				PreBuildingData = *Loadout.Buildings.Find(ABuilding);
			}
			PreBuildingData.Building = ABuilding;

			for (auto TempUp : UpgradesToAdd)
			{
				PreBuildingData.Upgrades.AddUnique(TempUp);
			}

			for (auto TempUp : UpgradesToRemove)
			{
				PreBuildingData.Upgrades.Remove(TempUp);
			}

			//Update the building data
			Loadout.Buildings.FindOrAdd(ABuilding) = PreBuildingData;
		}
	}

	//if it made any change then broadcast the changes for the host
	if (UpgradesToAdd.Num() != 0 || UpgradesToRemove.Num() != 0)
	{
		RemainingLevelRewardsUpdated.Broadcast();
		void OnRep_RemainingLevelRewards();

		LoadoutUpdated.Broadcast();
		void OnRep_Loadout();

	}

}

void ABDPlayerState::RefreshRemainingLevelRewards()
{
	if (Role == ROLE_Authority)
	{
		ABDGameState* GameState = Cast<ABDGameState>(GetWorld()->GetGameState());
		UBDGameInstance* GameInstance = Cast<UBDGameInstance>(GetGameInstance());
		int LevelRewards = 0;
		int LoadoutCost = 0;
		if (GameState != nullptr || GameInstance != nullptr)
		{
			LevelRewards = GameState->LevelRewards;

			for (auto It : Loadout.Buildings)
			{
				FBuildingData* Data = GameInstance->Buildings.Find(It.Value.Building);
				if (Data != nullptr)
				{
					for (auto Upgrade : It.Value.Upgrades)
					{
						FBuildingUpgrade BuidlingUp = *Data->LoadoutUpgrades.Find(Upgrade);
						LoadoutCost += BuidlingUp.Cost;
					}
				}
			}

			if (RemainingLevelRewards != LevelRewards - LoadoutCost)
			{
				RemainingLevelRewards = LevelRewards - LoadoutCost;
				RemainingLevelRewardsUpdated.Broadcast();
				void OnRep_RemainingLevelRewards();
			}
		}
	}
}

void ABDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	ABDGameState* GameState = Cast<ABDGameState>(GetWorld()->GetGameState());

	if (Role == ROLE_Authority && GameState != nullptr)
	{
		GameState->LevelRewardsUpdated.AddUObject(this, &ABDPlayerState::RefreshRemainingLevelRewards);
	}
	RefreshRemainingLevelRewards();

}

void ABDPlayerState::OnRep_RemainingLevelRewards()
{
	RemainingLevelRewardsUpdated.Broadcast();
}

void ABDPlayerState::OnRep_Loadout()
{
	LoadoutUpdated.Broadcast();
}
