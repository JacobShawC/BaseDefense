// Fill out your copyright notice in the Description page of Project Settings.

#include "StructLibrary.h"

template <typename T>
void ApplySingleBuff(const FBuildingBuffStruct& ABuff, T& Value)
{
	static_assert (std::is_same_v<T, int> || std::is_same_v<T, float>, "Trying to apply buff to non int/float");

	float Change = 0.0f;
	float InitialValue = Value;


	if (ABuff.Operator == EBuffOperator::Add)
	{
		Change = ABuff.Magnitude;
	}

	if (ABuff.Operator == EBuffOperator::Multiply)
	{
		Change = InitialValue * ABuff.Magnitude - InitialValue;
	}

	if (ABuff.Operator == EBuffOperator::Replace)
	{
		Value = ABuff.Magnitude;
		return;
	}

	Value += (T)Change;
}


FBuildingData FBuildingData::ReturnWithBuffs(TArray<FBuildingBuffStruct> ABuffs)
{
	FBuildingData BuffedBuildingData = *this;
	if (BuffedBuildingData.CanBeBuffed)
	{
		for (const FBuildingBuffStruct& Buff : ABuffs)
		{
			switch (Buff.Type)
			{
			case EBuildingBuffType::AttackSpeed: ApplySingleBuff(Buff, BuffedBuildingData.Attack.ReloadTime); break;
			case EBuildingBuffType::Bounty:; break;
			case EBuildingBuffType::ConstructionSpeed:ApplySingleBuff(Buff, BuffedBuildingData.ConstructionTime); break;
			case EBuildingBuffType::Cooldown:; break;
			case EBuildingBuffType::Cost: ApplySingleBuff(Buff, BuffedBuildingData.Cost); break;
			case EBuildingBuffType::CritMultiplier:; break;
			case EBuildingBuffType::Damage: ApplySingleBuff(Buff, BuffedBuildingData.Attack.Damage); break;
			case EBuildingBuffType::DotLength:; break;
			case EBuildingBuffType::DotMagnitude:; break;
			case EBuildingBuffType::Health: ApplySingleBuff(Buff, BuffedBuildingData.MaxHealth); break;
			case EBuildingBuffType::Income: ApplySingleBuff(Buff, BuffedBuildingData.Income.IncomeAmount); break;
			case EBuildingBuffType::Range: ApplySingleBuff(Buff, BuffedBuildingData.Attack.Range); break;
			case EBuildingBuffType::Regeneration: ApplySingleBuff(Buff, BuffedBuildingData.Regeneration.RegenAmount); break;
			case EBuildingBuffType::SlowPercent:; break;
			case EBuildingBuffType::SplashSize:; break;
			case EBuildingBuffType::UpgradeCost:; break;
			case EBuildingBuffType::UpgradeSpeed:; break;
			}
		}
	}

	return BuffedBuildingData;

}

TArray<FBuildingBuffStruct> FBuildingData::GetBuffsFromUpgrades(TArray<EBuildingUpgrade> AnUpgrades)
{
	TArray<FBuildingBuffStruct> ReturnBuffs;

	for (EBuildingUpgrade Upgrade : AnUpgrades)
	{
		TArray<FBuildingBuffStruct> FoundArray;
		if (Upgrades.Contains(Upgrade))
		{
			FoundArray = Upgrades.Find(Upgrade)->Upgrades;
			ReturnBuffs.Append(FoundArray);

		}
	}

	return ReturnBuffs;
}

FBuildingData FBuildingData::ReturnWithBuffs(TArray<FBuildingBuffStruct> ABuffs, TArray<EBuildingUpgrade> AnUpgrades)
{
	TArray<FBuildingBuffStruct> UpgradeBuffs = GetBuffsFromUpgrades(AnUpgrades);
	UpgradeBuffs.Append(ABuffs);
	return ReturnWithBuffs(UpgradeBuffs);
}

