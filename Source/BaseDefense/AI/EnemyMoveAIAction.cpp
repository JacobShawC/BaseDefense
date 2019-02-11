// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyMoveAIAction.h"
#include "EnemyChar.h"
#include "BDGameState.h"
#include "Engine/World.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "PlayerChar.h"
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

}

void UEnemyMoveAIAction::Activate()
{
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

//void UEnemyMoveAIAction::MoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result)
//{
//	
//}

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

	if (PlayerCharacters.Num() > 0)
	{
		int RandomNum = FMath::RandHelper(PlayerCharacters.Num() - 1);
		return PlayerCharacters[RandomNum];
	}
	return nullptr;
}

void UEnemyMoveAIAction::SetTarget()
{
	AIController->SetAggroTarget();
	if (AIController->AggroTarget != nullptr && !AIController->AggroTarget->IsPendingKill())
	{
		CurrentTarget = AIController->AggroTarget;
	}
	else if (!CurrentTarget || CurrentTarget->IsPendingKill() || CurrentTarget == nullptr || AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		//AEnemyChar* Actor = GetRandomPlayer();
		CurrentTarget = Cast<AActor>(GetRandomPlayer());
	}
	if (CurrentTarget)
	{
		FScriptDelegate Del;
		Del.BindUFunction(this, "SetTarget");
		CurrentTarget->OnDestroyed.AddUnique(Del);

	}
}
