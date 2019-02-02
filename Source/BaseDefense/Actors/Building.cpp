// Fill out your copyright notice in the Description page of Project Settings.
#include "Building.h"
#include "BDGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "FloatingBuildingInfo.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/BoxSphereBounds.h"
#include "Math/Box.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "PlayerChar.h"
#include "BuildingAIAction.h"
#include "BuildingAttackAIAction.h"
#include "UnrealNetwork.h"

// Sets default values
ABuilding::ABuilding()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//MeshComponent->SetSimulatePhysics(false);
	//MeshComponent->SetCollisionProfileName("Building");
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetIsReplicated(true);
	//MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	//MeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	//MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComponent->SetCollisionProfileName("BuildingMesh");

	MeshComponent->OnBeginCursorOver.AddDynamic(this, &ABuilding::OnMouseEnter);
	MeshComponent->OnEndCursorOver.AddDynamic(this, &ABuilding::OnMouseLeave);
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCustomDepthStencilValue(1);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->SetIsReplicated(true);

	OverlapCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlapCapsule"));
	OverlapCapsule->SetCollisionProfileName("Building");
	OverlapCapsule->SetGenerateOverlapEvents(true);
	OverlapCapsule->SetCapsuleSize(50, 50, true);
	OverlapCapsule->bVisible = true;
	OverlapCapsule->SetupAttachment(RootComponent);
	OverlapCapsule->SetRelativeLocation(FVector(0, 0, 25));
	FloatingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingInfo"));
	static ConstructorHelpers::FClassFinder<UUserWidget> FloatingWidgetClass(TEXT("WidgetBlueprint'/Game/UI/FloatingBuildingInfoWidget.FloatingBuildingInfoWidget_C'"));
	FloatingWidget->SetWidgetClass(FloatingWidgetClass.Class);
	FloatingWidget->SetDrawAtDesiredSize(true);
	FloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingWidget->SetupAttachment(RootComponent);
	FloatingWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABuilding::Initialise(EBuilding ABuilding)
{
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	Building = ABuilding;
	if (GameInstance)
	{
		BuildingData = *GameInstance->Buildings.Find(ABuilding);
		if (FloatingWidget)
		{
			FloatingInfo = Cast<UFloatingBuildingInfo>(FloatingWidget->GetUserWidgetObject());
		}
		if (FloatingInfo)
		{
			FloatingInfo->SetName(BuildingData.Name);
			FloatingInfo->SetNameVisibility(false);
		}
		HealthComponent->Initialise(BuildingData.MaxHealth);
		UStaticMesh* Mesh = nullptr;
		Mesh = GameInstance->Buildings.Find(ABuilding)->Mesh;
		if (Mesh)
		{
			MeshComponent->SetStaticMesh(Mesh);
			MeshComponent->SetRelativeScale3D(FVector(BuildingData.MeshScale));
			float MeshHeight = Mesh->GetBounds().BoxExtent.Z;
			MeshHeight = (MeshHeight * BuildingData.MeshScale) + 75;

			FloatingWidget->SetRelativeLocation(FVector(0, 0, MeshHeight));
		}
	}

	UBuildingAIAction* Action = nullptr;
	if (BuildingData.Attack.AttackType != EAttackType::None)
	{
		Action = NewObject<UBuildingAttackAIAction>();
		Action->Initialise(this);
		Actions.Add(Action);
		Action = nullptr;
	}

	if (Role == ROLE_Authority)
	{
		WhatDo();
	}
}

void ABuilding::Tick(float DeltaSeconds)
{
	if (Constructor && !Constructor->IsActorBeingDestroyed())
	{
		FVector PlayerLocation = Constructor->GetActorLocation();
		FVector BuildingLocation = GetActorLocation();
		float Distance = FVector::Dist(PlayerLocation, BuildingLocation);
		if (Distance > GameInstance->DefaultPlayerData.BuildRangeHorizontal)
		{
			SetActorTickEnabled(false);
			Destroy();
		}
		CurrentConstructionTime += DeltaSeconds;
		if (FloatingInfo)
		{
			FloatingInfo->SetConstruction(CurrentConstructionTime);
		}
		if (CurrentConstructionTime > ConstructedBuildingData.ConstructionTime)
		{
			SetUpBuilding(ConstructedBuildingData.Building);
			if (FloatingInfo)
			{
				FloatingInfo->SetConstructionVisibility(false);
			}
			SetActorTickEnabled(false);
		}
	}
	else
	{
		SetActorTickEnabled(false);
		Destroy();
	}
}

void ABuilding::Construct(EBuilding ABuilding, APlayerChar* AConstructor)
{
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	Constructor = AConstructor;

	Building = ABuilding;
	if (GameInstance)
	{
		SetUpBuilding(EBuilding::Construction);
		ConstructedBuildingData = *GameInstance->Buildings.Find(ABuilding);
		FloatingInfo->SetMaxConstruction(ConstructedBuildingData.ConstructionTime);

		SetActorTickEnabled(true);
	}
}

void ABuilding::SetUpBuilding(EBuilding ABuilding)
{
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	Building = ABuilding;
	if (GameInstance)
	{
		BuildingData = *GameInstance->Buildings.Find(ABuilding);
		if (FloatingWidget)
		{
			FloatingInfo = Cast<UFloatingBuildingInfo>(FloatingWidget->GetUserWidgetObject());
		}
		if (FloatingInfo)
		{
			FloatingInfo->SetName(BuildingData.Name);
			FloatingInfo->SetNameVisibility(false);
		}
		HealthComponent->Initialise(BuildingData.MaxHealth);
		UStaticMesh* Mesh = nullptr;
		Mesh = GameInstance->Buildings.Find(ABuilding)->Mesh;
		if (Mesh)
		{
			if (MeshComponent)
			{
				MeshComponent->SetStaticMesh(Mesh);
				MeshComponent->SetRelativeScale3D(FVector(BuildingData.MeshScale));
				float MeshHeight = Mesh->GetBounds().BoxExtent.Z;
				MeshHeight = (MeshHeight * BuildingData.MeshScale) + 75;

				FloatingWidget->SetRelativeLocation(FVector(0, 0, MeshHeight));
			}
		}
	}

	UBuildingAIAction* Action = nullptr;
	if (BuildingData.Attack.AttackType != EAttackType::None)
	{
		Action = NewObject<UBuildingAttackAIAction>();
		Actions.Add(Action);
		Action->Initialise(this);
		Action = nullptr;
	}
	if (Role == ROLE_Authority)
	{
		WhatDo();
	}

}

void ABuilding::OnMouseEnter(UPrimitiveComponent * TouchedComponent)
{
	if (FloatingInfo)
	{
		FloatingInfo->SetNameVisibility(true);
	}
	MeshComponent->SetRenderCustomDepth(true);
}

void ABuilding::OnMouseLeave(UPrimitiveComponent * TouchedComponent)
{
	if (FloatingInfo)
	{
		FloatingInfo->SetNameVisibility(false);
	}
	MeshComponent->SetRenderCustomDepth(false);
}

void ABuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuilding, MaxConstructionTime);
	DOREPLIFETIME(ABuilding, CurrentConstructionTime);
}

void ABuilding::WhatDo()
{
	if (CurrentAction != nullptr && CurrentAction->Executing && !CurrentAction->SafeToAbort)
	{
		return;
	}
	for (int i = 0; i < Actions.Num(); i++)
	{
		/*if (CurrentAction != nullptr && CurrentAction == Actions[i])
		{
			continue;
		}*/
		if (Actions[i]->ShouldActivate == true)
		{
			if (CurrentAction)
			{
				CurrentAction->Abort();
			}
			Actions[i]->Activate();
			CurrentAction = Actions[i];
			break;
		}
	}
}

void ABuilding::OnRep_SetConstruction()
{
	if (FloatingInfo)
	{
		FloatingInfo = Cast<UFloatingBuildingInfo>(FloatingWidget->GetUserWidgetObject());
	}

	if (FloatingWidget != nullptr)
	{
		FloatingInfo->SetConstruction(CurrentConstructionTime);
	}
}

void ABuilding::OnRep_SetMaxConstruction()
{
	if (FloatingWidget != nullptr)
	{
		FloatingInfo->SetMaxConstruction(MaxConstructionTime);
	}
}
