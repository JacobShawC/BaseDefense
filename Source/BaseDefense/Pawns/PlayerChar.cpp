// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerChar.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "FloatingInfo.h"
#include "Components/WidgetComponent.h"
#include "HealthComponent.h"
#include "BDGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "PlayerAction.h"
#include "ConstructAction.h"
#include "RepairAction.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "RepairAction.h"
// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	SpringArm->TargetArmLength = 800.f;
	SpringArm->RelativeRotation = FRotator(305.f, 0.f, 0.f);
	SpringArm->bDoCollisionTest = false;

	//Set up mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("SkeletalMesh'/Game/PolygonPirates/Meshes/Characters/People/SK_Character_Pirate_blackbeard_01.SK_Character_Pirate_blackbeard_01'"));
	GetMesh()->SetSkeletalMesh(PlayerMesh.Object, true);
	GetMesh()->SetReceivesDecals(false);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f), false);
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f), false);

	GetMesh()->SetGenerateOverlapEvents(true);
	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> AnimObj(TEXT("AnimBlueprint'/Game/PolygonPirates/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));
	//static ConstructorHelpers::FClassFinder<UAnimBlueprint> AnimBPClass(TEXT("AnimBlueprint'/Game/PolygonPirates/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));
	GetMesh()->AnimClass = AnimObj.Object;
	//GetMesh()->SetAnimInstanceClass(AnimObj.Object->GetAnimBlueprintGeneratedClass());
	//GetMesh()->SetAnimInstanceClass(AnimObj.Object->GetClass());
	// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	//GetCharacterMovement()->bConstrainToPlane = true;
	//GetCharacterMovement()->bSnapToPlaneAtStart = true;
	//GetCharacterMovement()->bUseControllerDesiredRotation = false;
	////GetCharacterMovement()->bIgnoreBaseRotation = true;
	////bUseControllerRotationPitch = false;
	////bUseControllerRotationYaw = true;
	////bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultDecalMat(TEXT("Material'/Game/Decals/RangeDecal.RangeDecal'"));

	//RangeDecal = CreateDefaultSubobject<UDecalComponent>("Decal");
	//RangeDecal->SetRelativeRotation(FRotator(90, 0, 0));
	////DecalComponent->SetRelativeScale3D(FVector(0.2f));
	//RangeDecal->DecalSize = FVector(400, 400, 400);
	//RangeDecal->FadeStartDelay = 0.0f;
	//RangeDecal->FadeScreenSize = 0.0f;
	//RangeDecal->FadeDuration = 0.0f;
	//RangeDecal->SetMaterial(0, DefaultDecalMat.Object);
	//RangeDecal->SetupAttachment(RootComponent);

	PlayerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PlayerCapsule"));
	PlayerCapsule->SetCollisionProfileName("Player");
	PlayerCapsule->SetGenerateOverlapEvents(true);
	PlayerCapsule->SetCapsuleSize(50, 50, true);
	PlayerCapsule->bVisible = true;
	PlayerCapsule->SetupAttachment(RootComponent);

	FloatingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingInfo"));
	static ConstructorHelpers::FClassFinder<UUserWidget> FloatingWidgetClass(TEXT("WidgetBlueprint'/Game/UI/FloatingPlayerInfoWidget.FloatingPlayerInfoWidget_C'"));
	FloatingWidget->SetWidgetClass(FloatingWidgetClass.Class);
	FloatingWidget->SetDrawAtDesiredSize(true);
	FloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingWidget->SetupAttachment(RootComponent);
	FloatingWidget->SetRelativeLocation(FVector(0, 0, 130));
	FloatingWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	ConstructAction = CreateDefaultSubobject<UConstructAction>(TEXT("ConstructAction"));
	RepairAction = CreateDefaultSubobject<URepairAction>(TEXT("RepairAction"));

	HealthComponent->SetNetAddressable(); // Make DSO components net addressable
	HealthComponent->SetIsReplicated(true); // Enable replication by default

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UBDGameInstance>(GetGameInstance());
	PlayerData = GameInstance->DefaultPlayerData;
	UFloatingInfo* Widget = Cast<UFloatingInfo>(FloatingWidget->GetUserWidgetObject());
	if (Widget)
	{
		if (GetPlayerState())
		{
			Widget->SetName(GetPlayerState()->GetPlayerName());
		}
	}
	HealthComponent->Initialise(PlayerData.MaxHealth);
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

