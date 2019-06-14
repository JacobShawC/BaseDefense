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
#include "UpgradeAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"

// Sets default values
APlayerChar::APlayerChar()
{


 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	SpringArm->TargetArmLength = 1200.f;
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
	GetMesh()->AnimClass = AnimObj.Object;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->AirControl = 0.2f;
	//GetCharacterMovement()->bConstrainToPlane = true;
	//GetCharacterMovement()->bSnapToPlaneAtStart = true;


	


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	

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
	UpgradeAction = CreateDefaultSubobject<UUpgradeAction>(TEXT("UpgradeAction"));

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

	if (FollowAction)
	{
		if (ActionTarget != nullptr && ActionTarget->IsValidLowLevelFast())
		{
			FRotator LookAtRotat = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ActionTarget->GetActorLocation());
			if (Controller != nullptr)
			{
				FRotator TempRotator = FRotator(0, LookAtRotat.Yaw, 0);
				SetActorRotation(TempRotator);
			}
		}
	}
}

bool APlayerChar::Kill()
{
	return true;
}

void APlayerChar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
}

void APlayerChar::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerChar::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerChar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerChar, ActionTarget);
	DOREPLIFETIME(APlayerChar, FollowAction);
	DOREPLIFETIME(APlayerChar, CurrentAction);


}

