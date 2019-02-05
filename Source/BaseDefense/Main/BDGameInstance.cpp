// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
UBDGameInstance::UBDGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> GUIBP(TEXT("WidgetBlueprint'/Game/UI/GUIWidget.GUIWidget_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> SlotBP(TEXT("WidgetBlueprint'/Game/UI/SlotWidget.SlotWidget_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> HotbarSlotBP(TEXT("WidgetBlueprint'/Game/UI/HotbarSlotWidget.HotbarSlotWidget_C'"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelMesh(TEXT("/Game/PolygonPirates/Meshes/Props/SM_Prop_Barrel_04.SM_Prop_Barrel_04"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonMesh(TEXT("StaticMesh'/Game/PolygonPirates/Meshes/Props/SM_Prop_Cannon_03.SM_Prop_Cannon_03'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CoveredCrate(TEXT("StaticMesh'/Game/PolygonPirates/Meshes/Props/SM_Prop_Crate_Covered_01.SM_Prop_Crate_Covered_01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonBall(TEXT("StaticMesh'/Game/PolygonPirates/Meshes/Props/SM_Prop_CannonBalls_01.SM_Prop_CannonBalls_01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Flowers(TEXT("StaticMesh'/Game/PolygonPirates/Meshes/Environments/SM_Env_Flowers_02.SM_Env_Flowers_02'"));
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> BarrelImage(TEXT("Texture2D'/Game/Textures/Icons/Completed/BarrelIcon.BarrelIcon'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> CannonImage(TEXT("Texture2D'/Game/Textures/Icons/Completed/CannonIcon.CannonIcon'"));

	
	Widgets.Add("GUI", GUIBP.Class);
	Widgets.Add("Slot", SlotBP.Class);
	Widgets.Add("HotbarSlot", HotbarSlotBP.Class);

	Images.Add(EBuilding::Wall, BarrelImage.Object);
	//----------------------------------------------------------------------------------------------------------
	//Buildings
	//----------------------------------------------------------------------------------------------------------

	DefaultPlayerData.MaxHealth = 200;

	//BaseBuilding
	FBuildingData BaseBuildingData;
	BaseBuildingData.Team = ETeam::Ally;
	BaseBuildingData.Mesh = BarrelMesh.Object;
	BaseBuildingData.Thumbnail = BarrelImage.Object;



	//Construction
	FBuildingData ConstructionData = BaseBuildingData;
	ConstructionData.Name = "Construction";
	ConstructionData.Mesh = CoveredCrate.Object;
	ConstructionData.Thumbnail = BarrelImage.Object;
	ConstructionData.Building = EBuilding::Construction;
	ConstructionData.MaxHealth = 1;
	ConstructionData.Cost = 0;
	ConstructionData.ConstructionTime = 0.0f;

	
	Buildings.Add(EBuilding::Construction, ConstructionData);


	//Wall
	FBuildingData WallBuildingData = BaseBuildingData;
	WallBuildingData.Name = "Wall";
	WallBuildingData.Mesh = BarrelMesh.Object;
	WallBuildingData.Thumbnail = BarrelImage.Object;
	WallBuildingData.Building = EBuilding::Wall;
	WallBuildingData.MaxHealth = 100;
	WallBuildingData.Cost = 20;
	WallBuildingData.ConstructionTime = 0.1f;

	WallBuildingData.Properties.Add(EBuildingProperty::Regen);
	WallBuildingData.Regeneration.RegenAmount = 1;

	Buildings.Add(EBuilding::Wall, WallBuildingData);

	//Farm
	FBuildingData FarmBuildingData = BaseBuildingData;
	FarmBuildingData.Name = "Farm";
	FarmBuildingData.Mesh = Flowers.Object;
	FarmBuildingData.Building = EBuilding::Farm;
	FarmBuildingData.MaxHealth = 50;
	FarmBuildingData.Cost = 100;
	FarmBuildingData.ConstructionTime = 4.f;

	FarmBuildingData.Properties.Add(EBuildingProperty::Regen);
	FarmBuildingData.Regeneration.RegenAmount = 1;

	FarmBuildingData.Properties.Add(EBuildingProperty::Income);
	FarmBuildingData.Income.IncomeAmount = 5;
	FarmBuildingData.Income.Cooldown = 2;

	Buildings.Add(EBuilding::Farm, FarmBuildingData);


	//Arrow Tower
	FBuildingData ArrowTowerBuildingData = BaseBuildingData;

	ArrowTowerBuildingData.Name = "Cannon";
	ArrowTowerBuildingData.Mesh = CannonMesh.Object;
	ArrowTowerBuildingData.Thumbnail = CannonImage.Object;
	ArrowTowerBuildingData.MeshScale = 0.4f;
	ArrowTowerBuildingData.Building = EBuilding::ArrowTower;
	ArrowTowerBuildingData.MaxHealth = 25;
	ArrowTowerBuildingData.Cost = 50;
	ArrowTowerBuildingData.ConstructionTime = 0.5f;

	ArrowTowerBuildingData.Properties.Add(EBuildingProperty::Regen);
	ArrowTowerBuildingData.Regeneration.RegenAmount = 1;
	ArrowTowerBuildingData.Regeneration.Cooldown = 1;

	ArrowTowerBuildingData.Properties.Add(EBuildingProperty::Attack);
	ArrowTowerBuildingData.Attack.AnimationTime = 1;
	ArrowTowerBuildingData.Attack.AttackRule = EAttackRule::Closest;
	ArrowTowerBuildingData.Attack.AttackType = EAttackType::Ranged;
	ArrowTowerBuildingData.Attack.Damage = 5;
	ArrowTowerBuildingData.Attack.ReloadTime = 1;
	ArrowTowerBuildingData.Attack.Range = 200;
	ArrowTowerBuildingData.Attack.Projectile.Mesh = CannonBall.Object;
	ArrowTowerBuildingData.Attack.Projectile.Speed = 100;

	Buildings.Add(EBuilding::ArrowTower, ArrowTowerBuildingData);



	//----------------------------------------------------------------------------------------------------------
	//Enemies
	//----------------------------------------------------------------------------------------------------------
	//Snall Zombie
	FEnemyData SmallZombieData;
	SmallZombieData.Name = "Small Zombie";
	SmallZombieData.Enemy = EEnemy::SmallZombie;
	SmallZombieData.MaxHealth = 50;
	SmallZombieData.MovementSpeed = 5;
	SmallZombieData.Attack.Damage = 10;
	SmallZombieData.Attack.ReloadTime = 2;
	SmallZombieData.Attack.Range = 50;
	SmallZombieData.Bounty = 50;
	SmallZombieData.Attack.AttackRule = EAttackRule::Closest;
	Enemies.Add(EEnemy::SmallZombie, SmallZombieData);
}