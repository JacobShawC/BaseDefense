	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "StructLibrary.generated.h"

/**
 * 
 */
UENUM()
enum class ELevelEvent : uint8
{
	None 		UMETA(DisplayName = "None"),
	Wave		UMETA(DisplayName = "Wave"),
	Event 		UMETA(DisplayName = "Event"),
	Boss 		UMETA(DisplayName = "Boss"),
};

UENUM()
enum class EBuffOperator : uint8
{
	None 		UMETA(DisplayName = "None"),
	Add			UMETA(DisplayName = "Add"),
	Multiply 	UMETA(DisplayName = "Multiply"),
};

UENUM()
enum class EBuildingBuffType : uint8
{
	None 				UMETA(DisplayName = "None"),
	Health				UMETA(DisplayName = "Health"),
	Damage 				UMETA(DisplayName = "Damage"),
	Cooldown 			UMETA(DisplayName = "Cooldown"),
	AttackSpeed 		UMETA(DisplayName = "AttackSpeed"),
	SlowPercent		 	UMETA(DisplayName = "SlowPercent"),
	SplashSize		 	UMETA(DisplayName = "SplashSize"),
	DotMagnitude		UMETA(DisplayName = "DotMagnitude"),
	DotLength		 	UMETA(DisplayName = "DotLength"),
	CritMultiplier		UMETA(DisplayName = "CritMultiplier"),
	Range			 	UMETA(DisplayName = "Range"),
	ConstructionSpeed 	UMETA(DisplayName = "ConstructionSpeed"),
	Income 				UMETA(DisplayName = "Income"),
	Cost 				UMETA(DisplayName = "Cost"),
	UpgradeCost 		UMETA(DisplayName = "UpgradeCost"),
	Regeneration 		UMETA(DisplayName = "Regeneration"),
	UpgradeSpeed 		UMETA(DisplayName = "UpgradeSpeed"),
	Bounty 				UMETA(DisplayName = "Bounty"),
};

UENUM()
enum class EPlayerBuffType : uint8
{
	None 				UMETA(DisplayName = "None"),
	Health				UMETA(DisplayName = "Health"),
	Cooldown 			UMETA(DisplayName = "Cooldown"),
	Damage 				UMETA(DisplayName = "Damage"),
	AttackSpeed 		UMETA(DisplayName = "AttackSpeed"),
	MovementSpeed		UMETA(DisplayName = "MovementSpeed"),
	ConstructionSpeed 	UMETA(DisplayName = "ConstructionSpeed"),
	Cost 				UMETA(DisplayName = "Cost"),
	UpgradeCost 		UMETA(DisplayName = "UpgradeCost"),
	Regeneration 		UMETA(DisplayName = "Regeneration"),
	UpgradeSpeed 		UMETA(DisplayName = "UpgradeSpeed"),
	SellSpeed 			UMETA(DisplayName = "SellSpeed"),
};

UENUM()
enum class EPlayerWeaponBuffType : uint8
{
	None 				UMETA(DisplayName = "None"),
	Damage 				UMETA(DisplayName = "Damage"),
	AttackSpeed 		UMETA(DisplayName = "AttackSpeed"),
	Cost 				UMETA(DisplayName = "Cost"),

};

UENUM()
enum class EEnemyBuffType : uint8
{
	None 				UMETA(DisplayName = "None"),
	Health				UMETA(DisplayName = "Health"),
	Damage 				UMETA(DisplayName = "Damage"),
	AttackSpeed 		UMETA(DisplayName = "AttackSpeed"),
	MovementSpeed		UMETA(DisplayName = "MovementSpeed"),
	Regeneration 		UMETA(DisplayName = "Regeneration"),
};

USTRUCT()
struct FBuildingBuffStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	EBuildingBuffType Type = EBuildingBuffType::None;
	
	UPROPERTY()
	EBuffOperator Operator = EBuffOperator::None;

	UPROPERTY()
	float Magnitude = 0.0f;

	UPROPERTY()
	float LifeSpan = 0.0f;
	
	UPROPERTY()
	class AActor* EffectHelper = nullptr;

	UPROPERTY()
	float Duration = 0;

	FBuildingBuffStruct();


	FBuildingBuffStruct(EBuildingBuffType AType, EBuffOperator AOperator, float AMagnitude, float ALifeSpan, class AActor* AEffectHelper, float ADuration)
	{
		Type = AType;
		Operator = AOperator;
		Magnitude = AMagnitude;
		LifeSpan = ALifeSpan;
		EffectHelper = AEffectHelper;
		Duration = ADuration;
	}

	FBuildingBuffStruct(EBuildingBuffType AType, EBuffOperator AOperator, float AMagnitude)
	{
		Type = AType;
		Operator = AOperator;
		Magnitude = AMagnitude;
	}
};


UENUM()
enum class EBuilding : uint8
{
	None 			UMETA(DisplayName = "None"),
	Construction 	UMETA(DisplayName = "Construction"),
	Wall 			UMETA(DisplayName = "Wall"),
	ArrowTower 		UMETA(DisplayName = "ArrowTower"),
	Farm			UMETA(DisplayName = "Farm"),
	SlowTower		UMETA(DisplayName = "SlowTower")
};

UENUM()
enum class EPlayerAction : uint8
{
	None 			UMETA(DisplayName = "None"),
	Constructing	UMETA(DisplayName = "Constructing"),
	Repairing		UMETA(DisplayName = "Repairing"),
	Selling 		UMETA(DisplayName = "Selling"),
};


UENUM()
enum class ETeam : uint8
{
	None 			UMETA(DisplayName = "None"),
	Ally			UMETA(DisplayName = "Ally"),
	Enemy 			UMETA(DisplayName = "Enemy")
};



UENUM()
enum class EBuildingType : uint8
{
	None 			UMETA(DisplayName = "None"),
	Wall 			UMETA(DisplayName = "Wall"),
	Tower 			UMETA(DisplayName = "Tower"),
	Farm			UMETA(DisplayName = "Farm")
};

UENUM()
enum class EBuildingProperty : uint8
{
	Attack 			UMETA(DisplayName = "Attack"),
	Income 			UMETA(DisplayName = "Income"),
	Regen 			UMETA(DisplayName = "Regen")
};

UENUM()
enum class EAttackProperty : uint8
{
	Slow 			UMETA(DisplayName = "Slow"),
	DOT				UMETA(DisplayName = "DOT"),
	AOE				UMETA(DisplayName = "AOE"),
	Pierce			UMETA(DisplayName = "Pierce")
};

UENUM()
enum class EAttackType : uint8
{
	None 			UMETA(DisplayName = "None"),
	Melee 			UMETA(DisplayName = "Melee"),
	Ranged 			UMETA(DisplayName = "Ranged")
};

UENUM()
enum class EAttackRule : uint8
{
	Closest 			UMETA(DisplayName = "Closest"),
	Furthest 			UMETA(DisplayName = "Furthest"),
	LowestHealth		UMETA(DisplayName = "LowestHealth"),
	HighestHealth		UMETA(DisplayName = "HighestHealth")
};

USTRUCT()
struct FIncome
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool RequiresSource = false;

	UPROPERTY()
	float IncomeAmount = 0;

	UPROPERTY()
		float Cooldown = 1;
};

USTRUCT()
struct FRegen
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool DamageStops = false;

	UPROPERTY()
	float Cooldown = 1;

	UPROPERTY()
	float RegenAmount = 0;

};

USTRUCT()
struct FDOT
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int Damage = 0;

	UPROPERTY()
	float Frequency = 0;

	UPROPERTY()
	float Length = 0;
};

USTRUCT()
struct FSlow
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float SlowPercent = 0;

	UPROPERTY()
	float Length = 0;
};

USTRUCT()
struct FProjectileData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Speed = 1;

	UPROPERTY()
	class UStaticMesh* Mesh = nullptr;

};

USTRUCT()
struct FAttack
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FProjectileData Projectile;

	UPROPERTY()
	TArray<EAttackProperty> AttackProperties;

	UPROPERTY()
	EAttackRule AttackRule = EAttackRule::Closest;

	UPROPERTY()
	float Damage = 0;

	UPROPERTY()
	float AnimationTime = 0;

	UPROPERTY()
	float ReloadTime = 0;

	UPROPERTY()
	EAttackType AttackType = EAttackType::None;

	UPROPERTY()
	float Range = 100;

	UPROPERTY()
	FDOT Dot;

	UPROPERTY()
	FSlow Slow;

};

UENUM()
enum class EEnemy : uint8
{
	None 			UMETA(DisplayName = "None"),
	Seaman 	UMETA(DisplayName = "Seaman"),
	FirstMate 	UMETA(DisplayName = "FirstMate"),
	FemalePirate 	UMETA(DisplayName = "FemalePirate")
};

USTRUCT()
struct FBuildingData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Name = "NoName";

	UPROPERTY()
	EBuilding Building = EBuilding::None;
	
	UPROPERTY()
	ETeam Team = ETeam::None;

	UPROPERTY()
	class UStaticMesh* Mesh = nullptr;

	UPROPERTY()
	float MeshScale = 1;

	UPROPERTY()
	UTexture2D* Thumbnail = nullptr;

	UPROPERTY()
	EBuildingType BuildingType = EBuildingType::None;

	UPROPERTY()
	float MaxHealth = 100;

	UPROPERTY()
	float Cost = 0;

	UPROPERTY()
	float ConstructionTime = 0;

	UPROPERTY()
	TArray<EBuildingProperty> Properties;

	UPROPERTY()
	FAttack Attack;

	UPROPERTY()
	FIncome Income;

	UPROPERTY()
	FRegen Regeneration;

	FBuildingData ReturnWithBuffs(TArray<FBuildingBuffStruct> Buffs);


	
};



USTRUCT()
struct FEnemyData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Name = "NoName";

	UPROPERTY()
	EEnemy Enemy = EEnemy::None;

	UPROPERTY()
	float MaxHealth = 0;

	UPROPERTY()
	float MovementSpeed = 0;

	UPROPERTY()
	class USkeletalMesh* Mesh = nullptr;
	
	UPROPERTY()
	class UAnimBlueprintGeneratedClass* Anim = nullptr;

	UPROPERTY()
	float Bounty = 0;

	UPROPERTY()
	FAttack Attack;
};

USTRUCT()
struct FPlayerData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<EBuilding> HotbarSlots;

	UPROPERTY()
	int Money;
	
	UPROPERTY()
	float BuildRangeHorizontal = 400.0f;

	UPROPERTY()
	float MaxHealth = 100;

	UPROPERTY()
	float BuildRangeVertical = 100.0f;

	UPROPERTY()
	float RepairRange = 200.0f;

	//HPPerSecond
	UPROPERTY()
	float RepairSpeed = 20.0f;

	UPROPERTY()
	float RepairCost = 0.2f;
};

USTRUCT()
struct FBuildingLocationInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool Buildable = false;
	UPROPERTY()
	bool Reachable = false;

	UPROPERTY()
	bool ClearFromBuildings = false;

	UPROPERTY()
	bool EvenSurface = false;

	UPROPERTY()
	bool RoundedVector = false;
};


USTRUCT()
struct FLevelEvent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	ELevelEvent Event = ELevelEvent::None;

	UPROPERTY()
	float StartTime = 0;

	UPROPERTY()
	float Duration = 0;
};

