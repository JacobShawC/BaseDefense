// Fill out your copyright notice in the Description page of Project Settings.

#include "UpgradeAction.h"
#include "Building.h"
#include "BDPlayerState.h"
#include "BDPlayerController.h"
#include "PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"

UUpgradeAction::UUpgradeAction(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

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
		EBuildingUpgrade NextUpgrade = EBuildingUpgrade::None;
			switch (ABuilding->CurrentUpgrade)
			{
			case EBuildingUpgrade::None: NextUpgrade = EBuildingUpgrade::Level2; break;
			case EBuildingUpgrade::Level2: NextUpgrade = EBuildingUpgrade::Level3; break;
			case EBuildingUpgrade::Level3: NextUpgrade = EBuildingUpgrade::Level4; break;
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

	CurrentBuilding = nullptr;
	PlayerChar->FollowAction = false;
	PlayerChar->ActionTarget = nullptr;

}

void UUpgradeAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentBuilding == nullptr || !CurrentBuilding->IsValidLowLevelFast() || CurrentBuilding->IsPendingKill())
	{
		CancelAction();
		return;
	}

	if (CurrentBuilding->CurrentInteraction.Type != EBuildingInteractionType::Upgrade || CurrentBuilding->Interacting == false)
	{
		CancelAction();
		return;
	}
	PlayerChar->ActionTarget = CurrentBuilding;
	PlayerChar->FollowAction = true;

}
