// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSController.h"
#include <GameFramework/Pawn.h>
#include "RTSPawn.h"
#include <GameFramework/Actor.h>
#include <TimerManager.h>
ARTSController::ARTSController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = false;
	PrimaryActorTick.bCanEverTick = true;

}

void ARTSController::SetupInputComponent()
{
	Super::SetupInputComponent();


	InputComponent->BindAction("Left Mouse", IE_Pressed, this, &ARTSController::SelectPressed);
	InputComponent->BindAction("Right Mouse", IE_Released, this, &ARTSController::SelectReleased);
	InputComponent->BindAction("Middle Mouse", IE_Pressed, this, &ARTSController::SelectAltPressed);
	InputComponent->BindAction("SelectAlt", IE_Released, this, &ARTSController::SelectAltReleased);

	InputComponent->BindAction("Hotbar1", IE_Pressed, this, &ARTSController::SelectHotbar<0>);
	InputComponent->BindAction("Hotbar2", IE_Pressed, this, &ARTSController::SelectHotbar<1>);
	InputComponent->BindAction("Hotbar3", IE_Pressed, this, &ARTSController::SelectHotbar<2>);
	InputComponent->BindAction("Hotbar4", IE_Pressed, this, &ARTSController::SelectHotbar<3>);
	InputComponent->BindAction("Hotbar5", IE_Pressed, this, &ARTSController::SelectHotbar<4>);
	InputComponent->BindAction("Hotbar6", IE_Pressed, this, &ARTSController::SelectHotbar<5>);
	InputComponent->BindAction("Hotbar7", IE_Pressed, this, &ARTSController::SelectHotbar<6>);
	InputComponent->BindAction("Hotbar8", IE_Pressed, this, &ARTSController::SelectHotbar<7>);
	InputComponent->BindAction("Hotbar9", IE_Pressed, this, &ARTSController::SelectHotbar<8>);
	InputComponent->BindAction("Hotbar10", IE_Pressed, this, &ARTSController::SelectHotbar<9>);

	InputComponent->BindAction("Scroll Up", IE_Pressed, this, &ARTSController::ScrollHotbar<false>);
	InputComponent->BindAction("Scroll Down", IE_Pressed, this, &ARTSController::ScrollHotbar<true>);
	SetInputMode(FInputModeGameAndUI());
}

void ARTSController::BeginPlay()
{
	
	
}

void ARTSController::ServerUpdatePositions_Implementation(FVector APawnPosition, FVector AMousePosition)
{
	Cast<ARTSPawn>(GetPawn())->PawnLocation = APawnPosition;
}

bool ARTSController::ServerUpdatePositions_Validate(FVector APawnPosition, FVector AMousePosition)
{
	return true;
}