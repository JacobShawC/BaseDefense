// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
UBDGameInstance::UBDGameInstance(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SeamanMesh(TEXT("SkeletalMesh'/Game/PolygonPirates/Meshes/Characters/People/SK_Character_Pirate_Seaman_01_Bare.SK_Character_Pirate_Seaman_01_Bare'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FirstMateMesh(TEXT("SkeletalMesh'/Game/PolygonPirates/Meshes/Characters/People/SK_Character_Pirate_First_Mate_01_Bare.SK_Character_Pirate_First_Mate_01_Bare'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FemalePirateMesh(TEXT("SkeletalMesh'/Game/PolygonPirates/Meshes/Characters/People/SK_Character_Female_Pirate_01_Bare.SK_Character_Female_Pirate_01_Bare'"));
	
	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> PirateSeamanAnimBP(TEXT("AnimBlueprint'/Game/PolygonPirates/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));




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
	ConstructionData.CanBeBuffed = false;
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

	//Wall Levels
	FBuildingUpgrade WallLevel2;
	FBuildingUpgrade WallLevel3;

	//Level 2
	WallLevel2.Upgrades.Add(FBuildingBuffStruct(EBuildingBuffType::Health, EBuffOperator::Multiply, 1.5));
	WallLevel2.UpgradeTime = 3.0f;
	WallBuildingData.Upgrades.Add(EBuildingUpgrade::Level2, WallLevel2);

	//Level 3
	WallLevel3.Upgrades.Add(FBuildingBuffStruct(EBuildingBuffType::Health, EBuffOperator::Multiply, 2));
	WallLevel3.UpgradeTime = 6.0f;

	WallBuildingData.Upgrades.Add(EBuildingUpgrade::Level3, WallLevel3);




	//Farm
	FBuildingData FarmBuildingData = BaseBuildingData;
	FarmBuildingData.Name = "Farm";
	FarmBuildingData.Mesh = Flowers.Object;
	FarmBuildingData.Building = EBuilding::Farm;
	FarmBuildingData.MaxHealth = 50;
	FarmBuildingData.Cost = 100;
	FarmBuildingData.ConstructionTime = 1.f;

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
	ArrowTowerBuildingData.Attack.Projectile.Speed = 300;

	Buildings.Add(EBuilding::ArrowTower, ArrowTowerBuildingData);

	//----------------------------------------------------------------------------------------------------------
	//Enemies
	//----------------------------------------------------------------------------------------------------------
	//Seaman
	FEnemyData SeamanData;
	SeamanData.Name = "Small Zombie";
	SeamanData.Enemy = EEnemy::Seaman;
	SeamanData.MaxHealth = 50;
	SeamanData.MovementSpeed = 5;
	SeamanData.Attack.Damage = 10;
	SeamanData.Attack.ReloadTime = 2;
	SeamanData.Attack.Range = 50;
	SeamanData.Bounty = 50;
	SeamanData.Mesh = SeamanMesh.Object;
	SeamanData.Anim = PirateSeamanAnimBP.Object;

	SeamanData.Attack.AttackRule = EAttackRule::Closest;
	Enemies.Add(EEnemy::Seaman, SeamanData);

	//First Mate
	FEnemyData FirstMateData;
	FirstMateData.Name = "Big Zombie";
	FirstMateData.Enemy = EEnemy::FirstMate;
	FirstMateData.MaxHealth = 200;
	FirstMateData.MovementSpeed = 5;
	FirstMateData.Attack.Damage = 20;
	FirstMateData.Attack.ReloadTime = 2;
	FirstMateData.Attack.Range = 50;
	FirstMateData.Bounty = 100;
	FirstMateData.Mesh = FirstMateMesh.Object;
	FirstMateData.Anim = PirateSeamanAnimBP.Object;

	FirstMateData.Attack.AttackRule = EAttackRule::Closest;
	Enemies.Add(EEnemy::FirstMate, FirstMateData);

	//Female Pirate
	FEnemyData FemalePirateData;
	FemalePirateData.Name = "Female Pirate";
	FemalePirateData.Enemy = EEnemy::FemalePirate;
	FemalePirateData.MaxHealth = 500;
	FemalePirateData.MovementSpeed = 5;
	FemalePirateData.Attack.Damage = 50;
	FemalePirateData.Attack.ReloadTime = 2;
	FemalePirateData.Attack.Range = 50;
	FemalePirateData.Bounty = 150;
	FemalePirateData.Mesh = FemalePirateMesh.Object;
	FemalePirateData.Anim = PirateSeamanAnimBP.Object;

	FemalePirateData.Attack.AttackRule = EAttackRule::Closest;
	Enemies.Add(EEnemy::FemalePirate, FemalePirateData);



}
