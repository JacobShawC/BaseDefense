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
#include "BDGameState.h"
#include "Math/Box.h"
#include "Engine/World.h"
#include "BDGameState.h"
#include "Public/TimerManager.h"
#include "UnrealNetwork.h"
#include "DamageTextActor.h"
#include "AnimationSharing/Public/AnimationSharingManager.h"
#include "AnimationSharing/Public/AnimationSharingSetup.h"


AEnemyChar::AEnemyChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("SkeletalMesh'/Game/PolygonPirates/Meshes/Characters/People/SK_Character_Pirate_Seaman_01_Bare.SK_Character_Pirate_Seaman_01_Bare'"));
	
	
	
	//GetMesh()->SetSkeletalMesh(PlayerMesh.Object, true);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f), false);
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f), false);
	GetMesh()->SetIsReplicated(true);
	GetMesh()->SetReceivesDecals(false);
	//static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> AnimObj(TEXT("AnimBlueprint'/Game/PolygonPirates/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));
	//static ConstructorHelpers::FClassFinder<UAnimationSharingSetup> AnimSharingBP(TEXT("AnimationSharingSetup'/Game/Animation/BDAnimationSharingSetup.BDAnimationSharingSetup'"));
	static ConstructorHelpers::FObjectFinder<UAnimationSharingSetup> AnimSharingBP(TEXT("AnimationSharingSetup'/Game/Animation/BDAnimationSharingSetup.BDAnimationSharingSetup'"));
	
	AnimSharingSetupClass = AnimSharingBP.Object;
	//GetMesh()->AnimClass = AnimObj.Object;
	GetMesh()->CastShadow = true;
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

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->SetIsReplicated(true);
	AIControllerClass = AEnemyAIController::StaticClass();
	
	FloatingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingInfo"));
	static ConstructorHelpers::FClassFinder<UUserWidget> FloatingWidgetClass(TEXT("WidgetBlueprint'/Game/UI/FloatingEnemyInfoWidget.FloatingEnemyInfoWidget_C'"));
	FloatingWidget->SetWidgetClass(FloatingWidgetClass.Class);
	FloatingWidget->SetDrawAtDesiredSize(true);
	FloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingWidget->SetupAttachment(RootComponent);
	FloatingWidget->SetRelativeLocation(FVector(0, 0, 130));
	FloatingWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	

	AutoPossessAI = EAutoPossessAI::Disabled;
}

void AEnemyChar::Initialise(EEnemy AnEnemy)
{
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	Enemy = AnEnemy;
	if (GameInstance)
	{
		EnemyData = *GameInstance->Enemies.Find(AnEnemy);
		GetMesh()->SetSkeletalMesh(EnemyData.Mesh, true);
		//GetMesh()->AnimClass = EnemyData.Anim;
		GetMesh()->SetAnimInstanceClass(EnemyData.Anim);
		//Animation sharing
		/*UAnimationSharingManager* AnimSharingManager = UAnimationSharingManager::GetAnimationSharingManager(GetWorld());
		if (AnimSharingManager == nullptr)
		{
			UAnimationSharingManager::CreateAnimationSharingManager(GetWorld(), AnimSharingSetupClass);
			AnimSharingManager = UAnimationSharingManager::GetAnimationSharingManager(GetWorld());

		}

		if (AnimSharingManager != nullptr)
		{
			FUpdateActorHandle TestActorHandle;
			AnimSharingManager->RegisterActorWithSkeleton(this, GetMesh()->SkeletalMesh->Skeleton, TestActorHandle);

		}*/

		Health = EnemyData.MaxHealth;
		Enemy = EnemyData.Enemy;
		//GetCharacterMovement()->MaxWalkSpeed = EnemyData.MovementSpeed;
		int i = 0;
		if (FloatingWidget)
		{
			FloatingInfo = Cast<UFloatingEnemyInfo>(FloatingWidget->GetUserWidgetObject());
			HealthComponent->Initialise(EnemyData.MaxHealth);
			float MeshHeight = GetMesh()->Bounds.BoxExtent.Z;
			FloatingHeight = MeshHeight + 10;

			OnRep_SetFloatingHeight();

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
	
	ABDGameState* GameState = Cast<ABDGameState>(GetWorld()->GetGameState());
	if (GameState != nullptr)
	{
		GameState->AddEnemyCharacter(TWeakObjectPtr<AEnemyChar>(this));

	}
	
	Initialise(EEnemy::Seaman);
	if (Role == ROLE_Authority)
	{
		SpawnDefaultController();
	}
}

void AEnemyChar::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Role == ROLE_Authority && EndPlayReason == EEndPlayReason::Destroyed)
	{
		OnKilled();
	}
	Super::EndPlay(EndPlayReason);
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

void AEnemyChar::OnKilled()
{
	ABDGameState* const MyGameState = GetWorld() != NULL ? GetWorld()->GetGameState<ABDGameState>() : NULL;

	if (MyGameState)
	{
		float DividedBounty = MyGameState->AddMoney(EnemyData.Bounty);
		MulticastSpawnDamageText(DividedBounty);
	}
}

void AEnemyChar::MulticastSpawnDamageText_Implementation(float ABounty)
{
	SpawnDamageText(FString::SanitizeFloat(ABounty), FColor::Yellow);
}

void AEnemyChar::SpawnDamageText(FString AText, FColor AColor)
{
	if (WasRecentlyRendered())
	{
		ADamageTextActor* DamageTextActor = nullptr;
		FVector OwnerLoc = GetActorLocation();
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		DamageTextActor = Cast<ADamageTextActor>(GetWorld()->SpawnActor<ADamageTextActor>(ADamageTextActor::StaticClass(), OwnerLoc, FRotator(0.0f), SpawnParams));
		if (DamageTextActor != nullptr)
		{
			DamageTextActor->Initialise(AText, AColor);
		}
	}

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
	int test = 3;
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

void AEnemyChar::OnRep_SetFloatingHeight()
{

	if (FloatingWidget != nullptr)
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().ClearTimer(FuzeTimerHandle);

		FloatingWidget->SetRelativeLocation(FVector(0, 0, FloatingHeight));
	}
	else
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &AEnemyChar::OnRep_SetFloatingHeight, 0.05f, false);
	}
}

void AEnemyChar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyChar, FloatingHeight);
}