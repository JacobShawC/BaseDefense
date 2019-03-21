// Fill out your copyright notice in the Description page of Project Settings.

#include "UpgradeAction.h"
#include "Building.h"
#include "BDPlayerState.h"
#include "BDPlayerController.h"
#include "PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"

bool UUpgradeAction::UpgradeBuilding(ABuilding* ABuilding)
{
	ABuilding->UpdateUpgradable();
	if (ABuilding->IsValidLowLevel() && ABuilding->Upgradable)
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
		EBuildingUpgrade NextUpgrade;
			switch (ABuilding->CurrentUpgrade)
			{
			case EBuildingUpgrade::None: NextUpgrade = EBuildingUpgrade::Level2;
			case EBuildingUpgrade::Level2: NextUpgrade = EBuildingUpgrade::Level3;
			case EBuildingUpgrade::Level3: NextUpgrade = EBuildingUpgrade::Level4;
			}
			float ACost = ABuilding->BuffedBuildingData.Upgrades.Find(NextUpgrade)->Cost;

		if (PlayerState->IsValidLowLevel() && PlayerChar != nullptr && PlayerChar->IsValidLowLevelFast() && PlayerChar->CurrentAction == nullptr && PlayerState->CanChangePlayerMoney(-ACost))
		{
			ABuilding->Upgrade(NextUpgrade, Cast<APlayerChar>(this->GetOwner()));

			PlayerState->ChangePlayerMoney(-ACost);
			PlayerChar->CurrentAction = this;

			CurrentBuilding = ABuilding;
			SetComponentTickEnabled(true);
			Upgrading = true;
			return true;
		}



	}

	Upgrading = false;
	return false;
}

void UUpgradeAction::CancelAction()
{
	SetComponentTickEnabled(false);
	Upgrading = false;
	if (PlayerChar->CurrentAction == this)
	{
		PlayerChar->CurrentAction = nullptr;
	}

	if (CurrentBuilding != nullptr && !CurrentBuilding->IsPendingKill() && CurrentBuilding->Interacting && CurrentBuilding->CurrentInteraction.Type == EBuildingInteractionType::Upgrade)
	{
		CurrentBuilding->CancelInteraction();
	}
}

void UUpgradeAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentBuilding == nullptr || !CurrentBuilding->IsValidLowLevelFast() || CurrentBuilding->IsPendingKill())
	{
		CancelAction();
		return;
	}

	if (CurrentBuilding->CurrentInteraction.Type != EBuildingInteractionType::Construct || CurrentBuilding->Interacting == false)
	{
		CancelAction();
		return;
	}
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
