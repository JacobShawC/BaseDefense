// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyMoveAIAction.h"
#include "EnemyChar.h"
#include "BDGameState.h"
#include "Engine/World.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "PlayerChar.h"
#include "EnemyChar.h"
#include "HealthComponent.h"
#include "Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "Templates/SharedPointer.h"
void UEnemyMoveAIAction::Initialise(AEnemyAIController* AnOwner)
{
	Super::Initialise(AnOwner);

	SafeToAbort = true;
	ShouldActivate = true;
	
	GameState = Cast<ABDGameState>(Char->GetWorld()->GetGameState());

	FScriptDelegate Del;
	Del.BindUFunction(this, "MoveComplete");
	AIController->OnMovementComplete.AddUnique(Del);
	SetTarget();
	
	AEnemyChar* EnemyChar = Cast<AEnemyChar>(AnOwner->GetPawn());
	UHealthComponent* EnemyHealth = Cast<UHealthComponent>(EnemyChar->HealthComponent);
	
	//TSharedRef< UEnemyMoveAIAction > EnemyMoveRef(this);

	//EnemyHealth->OnAttacked.AddSP(EnemyMoveRef, &UEnemyMoveAIAction::OnAttacked);
	EnemyHealth->OnAttacked.AddUObject(this, &UEnemyMoveAIAction::OnAttacked);
	NavSys = UNavigationSystemV1::GetCurrent(Char->GetWorld());
}

void UEnemyMoveAIAction::Activate()
{
	if (CurrentTarget == nullptr || !CurrentTarget->IsValidLowLevel() || CurrentTarget->IsPendingKill())
	{
		SetTarget();
	}

	if (CurrentTarget != nullptr && CurrentTarget->IsValidLowLevel() && !CurrentTarget->IsPendingKill())
	{
		AIController->MoveToActor(CurrentTarget);
		Executing = true;
	}
}

void UEnemyMoveAIAction::Abort()
{
	if (AIController)
	{
		AIController->StopMovement();
	}
	Executing = false;
}

void UEnemyMoveAIAction::MoveComplete(EPathFollowingResult::Type PathResult)
{
	if (PathResult == EPathFollowingResult::Type::Blocked)
	{
		Executing = false;
		AIController->WhatDo();
	}
}

class APlayerChar* UEnemyMoveAIAction::GetRandomPlayer()
{
	TArray<APlayerController*> PlayerControllers;
	TArray<APlayerChar*> PlayerCharacters;
	if (!GameState) return nullptr;
	
	PlayerCharacters = GameState->GetPlayerPawns();

	//we search for an alive player
	while (PlayerCharacters.Num() > 0)
	{
		int RandomNum = FMath::RandHelper(PlayerCharacters.Num() - 1);
		if (PlayerCharacters[RandomNum]->IsValidLowLevel() && !PlayerCharacters[RandomNum]->IsPendingKill())
		{
			return PlayerCharacters[RandomNum];
		}
		else
		{
			PlayerCharacters.RemoveAt(RandomNum);
		}
	}
	return nullptr;
}

void UEnemyMoveAIAction::SetTarget()
{
	if (!CurrentTarget || CurrentTarget->IsPendingKill() || CurrentTarget == nullptr || AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		//AEnemyChar* Actor = GetRandomPlayer();
		CurrentTarget = Cast<AActor>(GetRandomPlayer());
	}
	else
	{
		FScriptDelegate Del;
		Del.BindUFunction(this, "CurrentTargetDestroyed");
		CurrentTarget->OnDestroyed.AddUnique(Del);
	}
}

void UEnemyMoveAIAction::CurrentTargetDestroyed()
{
	TargetsChecked.Empty();
	CurrentTarget = nullptr;

	if (Executing)
	{
		Activate();
	}
	else
	{
		SetTarget();
	}
}

void UEnemyMoveAIAction::OnAttacked(AActor* AttackingTarget, float ADamage)
{
	if (AttackingTarget->IsValidLowLevel() && !AttackingTarget->IsPendingKill() && !TargetsChecked.Contains(AttackingTarget))
	{
		TargetsChecked.Add(AttackingTarget);

		/*UPathFollowingComponent* PathFollowingComponent = nullptr;
		PathFollowingComponent = AIController->GetPathFollowingComponent();
		FNavPathSharedPtr NavPTR = PathFollowingComponent->GetPath();
		float RemainingDistance = NavPTR->GetLength();*/

		FVector EnemyLocation = AIController->GetPawn()->GetActorLocation();

		FVector CurrentTargetLocation = CurrentTarget->GetActorLocation();

		FVector AttackingTargetLocation = AttackingTarget->GetActorLocation();


		float DistanceToCurrentTarget;
		float DistanceToAttackingTarget;

		NavSys->GetPathLength(EnemyLocation, CurrentTargetLocation, DistanceToCurrentTarget);
		NavSys->GetPathLength(EnemyLocation, AttackingTargetLocation, DistanceToAttackingTarget);

		if (DistanceToCurrentTarget > DistanceToAttackingTarget)
		{
			CurrentTarget = AttackingTarget;
			if (Executing)
			{
				Activate();
			}
		}

	}
}
