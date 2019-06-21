// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPawn.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/PawnMovementComponent.h>
#include <GameFramework/Pawn.h>
#include <GameFramework/Controller.h>
#include <Engine/World.h>
#include "RTSController.h"
#include <TimerManager.h>

// Sets default values
ARTSPawn::ARTSPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	MouseSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MouseSceneComponent"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SceneComponent);

	Camera->SetRelativeLocationAndRotation(FVector(-50, -50, 1500), FRotator(-70.f, 45.0f, 0));
	
	//GetMovementComponent()->Deactivate();


}

// Called when the game starts or when spawned
void ARTSPawn::BeginPlay()
{
	Super::BeginPlay();

	if (Role != ROLE_AutonomousProxy || !(Role == ROLE_Authority && GetController() != nullptr && GetWorld()->GetFirstPlayerController() == GetController()))
	{
		SetActorTickEnabled(false);
	}
	
}

void ARTSPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	InputComponent->BindAction("Left Mouse", IE_Pressed, this, &ARTSPawn::SelectPressed);
	InputComponent->BindAction("Right Mouse", IE_Released, this, &ARTSPawn::SelectReleased);
	InputComponent->BindAction("Middle Mouse", IE_Pressed, this, &ARTSPawn::SelectAltPressed);
	InputComponent->BindAction("SelectAlt", IE_Released, this, &ARTSPawn::SelectAltReleased);

	InputComponent->BindAction("Hotbar1", IE_Pressed, this, &ARTSPawn::SelectHotbar<0>);
	InputComponent->BindAction("Hotbar2", IE_Pressed, this, &ARTSPawn::SelectHotbar<1>);
	InputComponent->BindAction("Hotbar3", IE_Pressed, this, &ARTSPawn::SelectHotbar<2>);
	InputComponent->BindAction("Hotbar4", IE_Pressed, this, &ARTSPawn::SelectHotbar<3>);
	InputComponent->BindAction("Hotbar5", IE_Pressed, this, &ARTSPawn::SelectHotbar<4>);
	InputComponent->BindAction("Hotbar6", IE_Pressed, this, &ARTSPawn::SelectHotbar<5>);
	InputComponent->BindAction("Hotbar7", IE_Pressed, this, &ARTSPawn::SelectHotbar<6>);
	InputComponent->BindAction("Hotbar8", IE_Pressed, this, &ARTSPawn::SelectHotbar<7>);
	InputComponent->BindAction("Hotbar9", IE_Pressed, this, &ARTSPawn::SelectHotbar<8>);
	InputComponent->BindAction("Hotbar10", IE_Pressed, this, &ARTSPawn::SelectHotbar<9>);

	InputComponent->BindAction("Scroll Up", IE_Pressed, this, &ARTSPawn::ScrollHotbar<false>);
	InputComponent->BindAction("Scroll Down", IE_Pressed, this, &ARTSPawn::ScrollHotbar<true>);
}


void ARTSPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTSPawn, PawnLocation);
	DOREPLIFETIME(ARTSPawn, PointerLocation);
}

// Called every frame
void ARTSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//set the MouseSceneComponent location based on mouse location
}


void ARTSPawn::BeginPlay()
{
	// if we are a client pawn we want to set up the updating of our pawn positions ever 0.1 seconds
	if (Role == ROLE_AutonomousProxy)
	{
		FTimerHandle UniqueHandle;
		FTimerDelegate UpdatePositionsDelegate = FTimerDelegate::CreateUObject(this, &ARTSPawn::UpdatePositions);
		GetWorldTimerManager().SetTimer(UniqueHandle, UpdatePositionsDelegate, 0.1, true, 0.0f);
	}
}

//Update the server's representation of the client pawn location.
void ARTSPawn::UpdatePositions()
{
	if (GetController() != nullptr)
	{
		Cast<ARTSController>(GetController())->ServerUpdatePositions(GetActorLocation(), GetActorLocation());
	}
}

// Called to bind functionality to input
void ARTSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Update the pawns location for the replicated clients pawns but not the clients own pawn.
void ARTSPawn::OnRep_PawnLocation()
{
	

	if (GetController() != nullptr && GetController()->Role != ROLE_Authority)
	{
		SetActorLocation(PawnLocation);
	}
}

void ARTSPawn::OnRep_PointerLocation()
{
	if (GetController() != nullptr && GetController()->Role != ROLE_Authority)
	{

		//Set pointer location
	}

}


//return the vector based on whether the mouse is at the edge of the screen.
FVector ARTSPawn::GetCameaPanDirection()
{
	float MousePosX = 1;
	float MousePosY = 1;
	int ScreenSizeX = 100;
	int ScreenSizeY = 100;
	int CamDirectionX = 0;
	int CamDirectionY = 0;
	if (GetWorld() != nullptr )
	{
		GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosX, MousePosY);
		GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenSizeX, ScreenSizeY);
	}
	else
	{
		return FVector(0);
	}
	

	if (MousePosX <= 0)
	{
		CamDirectionX = -1;
	}
	if (MousePosX >= ScreenSizeX)
	{
		CamDirectionX = 1;
	}

	if (MousePosY <= 0)
	{
		CamDirectionY = -1;
	}
	if (MousePosY >= ScreenSizeY)
	{
		CamDirectionY = 1;
	}

	return FVector(CamDirectionX, CamDirectionY, 0);
}

//Update the actors location based on mouse movement if we are the client's own pawn or the host's own pawn
void ARTSPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Role == ROLE_AutonomousProxy || 
		(Role == ROLE_Authority && GetController() != nullptr && GetWorld()->GetFirstPlayerController() == GetController()))
	{
		AddActorWorldOffset(GetCameaPanDirection() * CamSpeed * DeltaSeconds);
	}
}





void ARTSPawn::SelectPressed()
{
}

void ARTSPawn::SelectReleased()
{
}

void ARTSPawn::SelectAltPressed()
{
}

void ARTSPawn::SelectAltReleased()
{
}


void ARTSPawn::MiddleMousePressed()
{
}



void ARTSPawn::MiddleMouseReleased()
{
}

void ARTSPawn::ScrollHotbar(bool UpOrDown)
{

}

void ARTSPawn::SelectHotbar(int ASlot)
{

}