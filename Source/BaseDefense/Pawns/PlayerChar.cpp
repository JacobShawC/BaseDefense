// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/PawnMovementComponent.h>
#include <GameFramework/Pawn.h>
#include <GameFramework/Controller.h>
#include <Engine/World.h>
#include "BDPlayerController.h"
#include <TimerManager.h>
#include "Net/UnrealNetwork.h"
#include "BDGameInstance.h"
#include <UserWidget.h>
#include "GUI.h"
#include "Hotbar.h"

// Sets default values
APlayerChar::APlayerChar()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bReplicates = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	MouseSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MouseSceneComponent"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SceneComponent);

	//Camera->SetRelativeLocationAndRotation(FVector(-600, -600, 2000), FRotator(-70.f, 45.0f, 0));
	Camera->SetRelativeLocationAndRotation(FVector(-600, -600, 2000), FRotator(-70.f, 45.0f, 0));
}


void APlayerChar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up game play key bindings
	check(PlayerInputComponent);
	InputComponent->BindAction("Left Mouse", IE_Pressed, this, &APlayerChar::LeftMousePressed);
	InputComponent->BindAction("Left Mouse", IE_Released, this, &APlayerChar::LeftMouseReleased);
	InputComponent->BindAction("Middle Mouse", IE_Pressed, this, &APlayerChar::MiddleMousePressed);
	InputComponent->BindAction("Middle Mouse", IE_Released, this, &APlayerChar::MiddleMouseReleased);

	InputComponent->BindAction("Right Mouse", IE_Pressed, this, &APlayerChar::RightMousePressed);
	InputComponent->BindAction("Right Mouse", IE_Released, this, &APlayerChar::RightMouseReleased);

	InputComponent->BindAction("Hotbar1", IE_Pressed, this, &APlayerChar::SelectHotbar<0>);
	InputComponent->BindAction("Hotbar2", IE_Pressed, this, &APlayerChar::SelectHotbar<1>);
	InputComponent->BindAction("Hotbar3", IE_Pressed, this, &APlayerChar::SelectHotbar<2>);
	InputComponent->BindAction("Hotbar4", IE_Pressed, this, &APlayerChar::SelectHotbar<3>);
	InputComponent->BindAction("Hotbar5", IE_Pressed, this, &APlayerChar::SelectHotbar<4>);
	InputComponent->BindAction("Hotbar6", IE_Pressed, this, &APlayerChar::SelectHotbar<5>);
	InputComponent->BindAction("Hotbar7", IE_Pressed, this, &APlayerChar::SelectHotbar<6>);
	InputComponent->BindAction("Hotbar8", IE_Pressed, this, &APlayerChar::SelectHotbar<7>);
	InputComponent->BindAction("Hotbar9", IE_Pressed, this, &APlayerChar::SelectHotbar<8>);
	InputComponent->BindAction("Hotbar10", IE_Pressed, this, &APlayerChar::SelectHotbar<9>);
	InputComponent->BindAction("Scroll Up", IE_Pressed, this, &APlayerChar::ScrollHotbar<false>);
	InputComponent->BindAction("Scroll Down", IE_Pressed, this, &APlayerChar::ScrollHotbar<true>);
}


void APlayerChar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerChar, PawnLocation);
	DOREPLIFETIME(APlayerChar, PointerLocation);
}

void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(-12750.0f, 12750.0f, 0));
	SetActorTickEnabled(true);

	// if we are a client pawn we want to set up the updating of our pawn positions ever 0.1 seconds
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		FTimerHandle UniqueHandle;
		FTimerDelegate UpdatePositionsDelegate = FTimerDelegate::CreateUObject(this, &APlayerChar::UpdatePositions);
		GetWorldTimerManager().SetTimer(UniqueHandle, UpdatePositionsDelegate, 0.1, true, 0.0f);
	}

	

}

//Update the server's representation of the client pawn location.
void APlayerChar::UpdatePositions()
{
	if (GetController() != nullptr)
	{
		Cast<ABDPlayerController>(GetController())->ServerUpdatePositions(GetActorLocation(), GetActorLocation());
	}
}

//Update the pawns location for the replicated clients pawns but not the clients own pawn.
void APlayerChar::OnRep_PawnLocation()
{


	//if (GetController() != nullptr && GetController()->Role != ROLE_Authority)
	if (GetController() == nullptr)
	{
		SetActorLocation(PawnLocation);
	}
}

void APlayerChar::OnRep_PointerLocation()
{
	if (GetController() == nullptr)
	{

		//Set pointer location
	}

}




//return the vector based on whether the mouse is at the edge of the screen.
FVector APlayerChar::GetCameraPanDirection()
{
	int Edge = 7;
	FVector ActorLocation = GetActorLocation();
	float MousePosX = Edge + 1;
	float MousePosY = Edge + 1;
	int ScreenSizeX = 100;
	int ScreenSizeY = 100;
	int CamDirectionX = 0;
	int CamDirectionY = 0;
	if (GetWorld() != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosX, MousePosY);
		GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenSizeX, ScreenSizeY);
	}
	else
	{
		return FVector(0);
	}
	if (MousePosX != -1)
	{
		if (MousePosX <= 0 + Edge)
		{
			CamDirectionX = -1;
		}

		if (MousePosX >= ScreenSizeX - Edge)
		{
			CamDirectionX = 1;
		}

		if (MousePosY <= 0 + Edge)
		{
			CamDirectionY = -1;
		}
		if (MousePosY >= ScreenSizeY - Edge)
		{
			CamDirectionY = 1;
		}

		if (MousePosX <= 0 + Edge)
		{
			CamDirectionX = -1;
		}


	}

	FVector ReturnVector = FVector(CamDirectionX, CamDirectionY, 0);
	return ReturnVector;
}

void APlayerChar::LeftMousePressed()
{
}

void APlayerChar::LeftMouseReleased()
{
}

void APlayerChar::RightMousePressed()
{
}

void APlayerChar::RightMouseReleased()
{
}

//Update the actors location based on mouse movement if we are the client's own pawn or the host's own pawn
void APlayerChar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	

	if (GetController() != nullptr && GetController()->IsLocalPlayerController())
	{
		FVector PanDirection = -(GetCameraPanDirection() * CamSpeed * DeltaSeconds) * 50;

		PanDirection = PanDirection.RotateAngleAxis(-45, FVector(0, 0, 1)); 
		AddActorWorldOffset(PanDirection);

		FVector ActorLocation = GetActorLocation();


		//check if actor is hitting edge of map
		if (ActorLocation.X > 0)
		{
			ActorLocation.X = 0;
		}

		if (ActorLocation.X < -25400)
		{
			ActorLocation.X = -25400;
		}

		if (ActorLocation.Y > 25400)
		{
			ActorLocation.Y = 25400;
		}

		if (ActorLocation.Y < 0)
		{
			ActorLocation.Y = 0;
		}
		SetActorLocation(ActorLocation);
	}
}






void APlayerChar::MiddleMousePressed()
{
}



void APlayerChar::MiddleMouseReleased()
{
}

void APlayerChar::ScrollHotbar(bool UpOrDown)
{
	ABDPlayerController* PlayerController = GetController<ABDPlayerController>();
	if (PlayerController->GUIWidget != nullptr)
	{
		PlayerController->GUIWidget->HotBar->Scroll(UpOrDown);
	}

}

void APlayerChar::SelectHotbar(int ASlot)
{
	ABDPlayerController* PlayerController = GetController<ABDPlayerController>();
	if (PlayerController->GUIWidget != nullptr)
	{
		PlayerController->GUIWidget->HotBar->Select(ASlot);
	}
}