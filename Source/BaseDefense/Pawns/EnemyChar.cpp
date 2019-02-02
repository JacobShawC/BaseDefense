// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyChar.h"
#include "BDGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "EnemyAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BDGameInstance.h"
#include "FloatingEnemyInfo.h"
#include "Components/WidgetComponent.h"
#include "HealthComponent.h"
#include "Math/BoxSphereBounds.h"
#include "Math/Box.h"// Sets default values
AEnemyChar::AEnemyChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("SkeletalMesh'/Game/PolygonPirates/Meshes/Characters/People/SK_Character_Pirate_Seaman_01_Bare.SK_Character_Pirate_Seaman_01_Bare'"));
	GetMesh()->SetSkeletalMesh(PlayerMesh.Object, true);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f), false);
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f), false);

	GetMesh()->SetReceivesDecals(false);
	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> AnimObj(TEXT("AnimBlueprint'/Game/PolygonPirates/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));
	GetMesh()->AnimClass = AnimObj.Object;
	GetMesh()->CastShadow = false;
	GetMesh()->SetGenerateOverlapEvents(false);
	//GetMesh()->SetCollisionProfileName("Enemy");
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetMesh()->OnBeginCursorOver.AddDynamic(this, &AEnemyChar::OnMouseEnter);
	GetMesh()->OnEndCursorOver.AddDynamic(this, &AEnemyChar::OnMouseLeave);
	GetMesh()->SetCustomDepthStencilValue(3);
	GetMesh()->bComponentUseFixedSkelBounds = true;
	GetMesh()->bEnableUpdateRateOptimizations = true;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	GetMesh()->bRenderStatic = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	OverlapCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlapCapsule"));
	OverlapCapsule->SetCollisionProfileName("Enemy");
	OverlapCapsule->SetGenerateOverlapEvents(true);
	OverlapCapsule->SetCapsuleSize(50, 50, true);
	OverlapCapsule->bVisible = true;
	OverlapCapsule->SetupAttachment(RootComponent);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	//GetCharacterMovement()->bUseRVOAvoidance = true;
	//GetCharacterMovement()->AvoidanceConsiderationRadius = 30;
	//GetCharacterMovement()->AvoidanceWeight = 0.5;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->bCanWalkOffLedges = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->UpdatedComponent = GetCapsuleComponent();
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = false;
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanWalk = true;
		GetCharacterMovement()->SetJumpAllowed(false);
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bAlwaysCheckFloor = false;
	}


	AIControllerClass = AEnemyAIController::StaticClass();
	
	FloatingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingInfo"));
	static ConstructorHelpers::FClassFinder<UUserWidget> FloatingWidgetClass(TEXT("WidgetBlueprint'/Game/UI/FloatingEnemyInfoWidget.FloatingEnemyInfoWidget_C'"));
	FloatingWidget->SetWidgetClass(FloatingWidgetClass.Class);
	FloatingWidget->SetDrawAtDesiredSize(true);
	FloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingWidget->SetupAttachment(RootComponent);
	FloatingWidget->SetRelativeLocation(FVector(0, 0, 130));
	FloatingWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->SetIsReplicated(true);

	AutoPossessAI = EAutoPossessAI::Disabled;
}

void AEnemyChar::Initialise(EEnemy AnEnemy)
{
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	Enemy = AnEnemy;
	if (GameInstance)
	{
		EnemyData = *GameInstance->Enemies.Find(AnEnemy);
		Health = EnemyData.MaxHealth;
		Enemy = EnemyData.Enemy;
		//GetCharacterMovement()->MaxWalkSpeed = EnemyData.MovementSpeed;
	
		if (FloatingWidget)
		{
			FloatingInfo = Cast<UFloatingEnemyInfo>(FloatingWidget->GetUserWidgetObject());
			HealthComponent->Initialise(EnemyData.MaxHealth);
			float MeshHeight = GetMesh()->Bounds.BoxExtent.Z;
			MeshHeight = (MeshHeight)+10;

			FloatingWidget->SetRelativeLocation(FVector(0, 0, MeshHeight));

		}
		if (FloatingInfo)
		{
			FloatingInfo->SetName(EnemyData.Name);
			//FloatingInfo->HideName();
			FloatingWidget->SetVisibility(false);
		}
		
	}
}

// Called when the game starts or when spawned
void AEnemyChar::BeginPlay()
{
	Super::BeginPlay();
	Initialise(EEnemy::SmallZombie);
	SpawnDefaultController();
}

// Called every frame
void AEnemyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyChar::Damaged(float ADamage)
{
	Health -= ADamage;
	if (Health <= 0)
	{
		Health = 0;
		Die();
	}

}

void AEnemyChar::Die()
{
	Destroy();
}

void AEnemyChar::OnMouseEnter(UPrimitiveComponent * TouchedComponent)
{
	if (FloatingWidget)
	{
		FloatingWidget->SetVisibility(true);
	}
	GetMesh()->SetRenderCustomDepth(true);


	/*if (FloatingInfo)
	{
		FloatingInfo->ShowName();
	}*/
}

void AEnemyChar::OnMouseLeave(UPrimitiveComponent * TouchedComponent)
{
	if (FloatingWidget)
	{
		FloatingWidget->SetVisibility(false);
	}
	GetMesh()->SetRenderCustomDepth(false);

	/*if (FloatingInfo)
	{
		FloatingInfo->HideName();
	}*/
}
