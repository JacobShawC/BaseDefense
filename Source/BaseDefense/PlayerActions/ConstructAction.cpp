// Fill out your copyright notice in the Description page of Project Settings.

#include "ConstructAction.h"
#include "Building.h"
#include "Engine/World.h"
#include "BDGameInstance.h"
#include "PlayerChar.h"
#include "Building.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "BDPlayerState.h"
#include "Kismet/KismetMathLibrary.h"
UConstructAction::UConstructAction(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

bool UConstructAction::ConstructBuilding(FBuildingData AData, FVector APosition)
{
	if (PlayerChar == nullptr)
	{
		PlayerChar = Cast<APlayerChar>(GetOwner());
	}

	if (PlayerState == nullptr)
	{
		Controller = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
		PlayerState = Cast<ABDPlayerState>(Controller->PlayerState);
	}
	
	if (PlayerState->IsValidLowLevel() && PlayerChar != nullptr && PlayerChar->IsValidLowLevelFast() && PlayerChar->CurrentAction == nullptr && PlayerState->CanChangePlayerMoney(-AData.Cost))
	{
		FRotator Rotation = FRotator(0.0f);
		ABuilding* Building = nullptr;
		Building = GetWorld()->SpawnActor<ABuilding>(APosition, Rotation);

		if (Building != nullptr)
		{
			Building->Construct(AData.Building, PlayerChar);

			PlayerState->ChangePlayerMoney(-AData.Cost);
			Data = AData;
			PlayerChar->CurrentAction = this;
			CurrentBuilding = Building;
			SetComponentTickEnabled(true);
			Constructing = true;
			return true;
		}

	}
	Constructing = false;
	return false;
	

}
void UConstructAction::CancelConstruction()
{
	SetComponentTickEnabled(false);
	Constructing = false;
	if (PlayerChar->CurrentAction == this)
	{
		PlayerChar->CurrentAction = nullptr;
	}

	if (CurrentBuilding != nullptr && !CurrentBuilding->IsPendingKill() && CurrentBuilding->Interacting && CurrentBuilding->CurrentInteraction.Type == EBuildingInteractionType::Construct)
	{
		CurrentBuilding->CancelInteraction();
	}

	CurrentBuilding = nullptr;

	PlayerChar->FollowAction = false;
	PlayerChar->ActionTarget = nullptr;
}

void UConstructAction::CancelAction()
{
	CancelConstruction();
}

void UConstructAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentBuilding == nullptr || CurrentBuilding->IsPendingKill())
	{
		CancelConstruction();
		return;
	}

	if (CurrentBuilding->CurrentInteraction.Type != EBuildingInteractionType::Construct || CurrentBuilding->Interacting == false)
	{
		CancelConstruction();
		return;
	}
	PlayerChar->ActionTarget = CurrentBuilding;
	PlayerChar->FollowAction = true;
}
