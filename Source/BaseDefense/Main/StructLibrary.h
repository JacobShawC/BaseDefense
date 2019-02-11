	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "StructLibrary.generated.h"

/**
 * 
 */

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
	int IncomeAmount = 0;

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
	int Damage = 0;

	UPROPERTY()
	float AnimationTime = 0;

	UPROPERTY()
	float ReloadTime = 0;

	UPROPERTY()
	EAttackType AttackType = EAttackType::None;

	UPROPERTY()
	int Range = 100;

	UPROPERTY()
	FDOT Dot;

	UPROPERTY()
	FSlow Slow;

};

UENUM()
enum class EEnemy : uint8
{
	None 			UMETA(DisplayName = "None"),
	SmallZombie 	UMETA(DisplayName = "SmallZombie")
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
	int Cost = 0;

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
