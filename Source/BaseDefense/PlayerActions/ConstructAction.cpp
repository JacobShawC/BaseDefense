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

		if (Building != nullptr && PlayerChar->CurrentAction != nullptr)
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



void UConstructAction::CancelConstruction(EConstructionCancelReason AReason)
{
	SetComponentTickEnabled(false);
	Constructing = false;
	if (PlayerChar->CurrentAction == this)
	{
		PlayerChar->CurrentAction = nullptr;
	}

	if (AReason == EConstructionCancelReason::Cancelled || AReason == EConstructionCancelReason::Distance)
	{
		if (CurrentBuilding != nullptr && !CurrentBuilding->IsPendingKill())
		{
			CurrentBuilding->CancelConstruction();
			if (PlayerState == nullptr)
			{
				ABDPlayerState* PlayerState = nullptr;
				if (Controller == nullptr)
				{
					Controller = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
				}
				PlayerState = Cast<ABDPlayerState>(Controller->PlayerState);
			}
			if (PlayerState->IsValidLowLevelFast())
			{
				PlayerState->ChangePlayerMoney(Data.Cost);
			}


		}
	}
}

void UConstructAction::CancelAction()
{
	CancelConstruction(EConstructionCancelReason::Cancelled);
}

void UConstructAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentBuilding == nullptr || CurrentBuilding->IsPendingKill())
	{
		CancelConstruction(EConstructionCancelReason::Killed);
		return;
	}

	if (CurrentBuilding->Building != EBuilding::Construction)
	{
		CancelConstruction(EConstructionCancelReason::Finished);
		return;
	}



	if (GameInstance == nullptr)
	{
		GameInstance = Cast<UBDGameInstance>(GetWorld()->GetGameInstance());
	}
	if (GameInstance)
	{
		float Distance = FVector::Dist(GetOwner()->GetActorLocation(), CurrentBuilding->GetActorLocation());

		if (Distance > GameInstance->DefaultPlayerData.BuildRangeHorizontal)
		{
			CancelConstruction(EConstructionCancelReason::Distance);
			return;
		}
	}
	Constructing = true;
	FRotator LookAtRotat = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), CurrentBuilding->GetActorLocation());
	if (Controller == nullptr)
	{
		Controller = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
	}
	if (Controller != nullptr)
	{
		FRotator TempRotator = FRotator(0, LookAtRotat.Yaw, 0);
		GetOwner()->SetActorRotation(TempRotator);
	}
}
