// Fill out your copyright notice in the Description page of Project Settings.

#include "RepairAction.h"
#include "Building.h"
#include "PlayerChar.h"
#include "BDPlayerState.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "StructLibrary.h"
#include "HealthComponent.h"
#include "Engine/World.h"
#include "BDGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
URepairAction::URepairAction(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

bool URepairAction::RepairBuilding(ABuilding* ABuilding)
{
	if (ABuilding->IsValidLowLevelFast())
	{
		Building = ABuilding;
	}
	else
	{
		return false;
	}

	if (PlayerChar == nullptr)
	{
		PlayerChar = Cast<APlayerChar>(GetOwner());
	}
	if (PlayerChar == nullptr)
	{
		return false;
	}

	if (PlayerState == nullptr)
	{
		Controller = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
		PlayerState = Cast<ABDPlayerState>(Controller->PlayerState);
		PlayerData = PlayerState->PlayerData;
	}
	// players have repair speed in hp/s and repair cost per hp
	HealthComponent = nullptr;
	HealthComponent = Cast<UHealthComponent>(Building->GetComponentByClass(UHealthComponent::StaticClass()));

	if (HealthComponent != nullptr && PlayerState != nullptr && PlayerChar->CurrentAction != nullptr && HealthComponent->Health < HealthComponent->MaxHealth && PlayerState->Money > 0)
	{
		PlayerChar->CurrentAction = this;

		SetComponentTickEnabled(true);
		Repairing = true;
		return true;
	}
	return false;
}

void URepairAction::CancelRepair(ERepairCancelReason AReason)
{
	SetComponentTickEnabled(false);
	Repairing = false;
	if (PlayerChar->CurrentAction == this)
	{
		PlayerChar->CurrentAction = nullptr;
	}
}

void URepairAction::CancelAction()
{
	CancelRepair(ERepairCancelReason::Cancelled);
}

void URepairAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	if (Building == nullptr || Building->IsPendingKill())
	{
		CancelRepair(ERepairCancelReason::Killed);
		return;
	}

	if (GameInstance == nullptr)
	{
		GameInstance = Cast<UBDGameInstance>(GetWorld()->GetGameInstance());
	}

	if (PlayerState == nullptr)
	{
		Controller = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
		PlayerState = Cast<ABDPlayerState>(Controller->PlayerState);
		PlayerData = PlayerState->PlayerData;
	}
	


	if (GameInstance)
	{
		float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Building->GetActorLocation());

		if (Distance > PlayerData.RepairRange)
		{
			CancelRepair(ERepairCancelReason::Distance);
			return;
		}
	}
	else 
	{
		CancelRepair(ERepairCancelReason::Killed);
		return;
	}

	


	float MaxHealing = 0;
	float HealthToSpend = PlayerState->Money / PlayerData.RepairCost;
	float HealthCanHeal = 0;
	float TickHeal = DeltaTime * PlayerData.RepairCost;
	if (HealthComponent)
	{
		MaxHealing = HealthComponent->MaxHealth = HealthComponent->Health;
		if (TickHeal <= HealthToSpend)
		{
			HealthCanHeal = TickHeal;
		}
		else
		{
			HealthCanHeal = HealthToSpend;
		}

		if (HealthCanHeal > MaxHealing)
		{
			HealthCanHeal = MaxHealing;
		}
		if (HealthCanHeal > 0)
		{
			PlayerState->ChangePlayerMoney(-(HealthCanHeal * PlayerData.RepairCost));
			HealthComponent->Heal(HealthCanHeal);
		}
		else
		{
			CancelRepair(ERepairCancelReason::Finished);
			return;
		}
	}
	else
	{
		CancelRepair(ERepairCancelReason::Killed);
		return;
	}


	Repairing = true;
	FRotator LookAtRotat = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Building->GetActorLocation());
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
