// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"

#include "BDGameMode.h"
#include "EnemyChar.h"
#include "BDPlayerController.h"
#include "PlayerChar.h"
#include "BDGameState.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Building.h"
#include "HealthComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "EnemyAIAction.h"
#include "EnemyAttackAIAction.h"
#include "UObject/NoExportTypes.h"
#include "EnemyMoveAIAction.h"
#include "AIAction.h"
DEFINE_LOG_CATEGORY(EnemyAIController);


AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	
}

void AEnemyAIController::Possess(class APawn* inPawn)
{
	Super::Possess(inPawn);
	MyChar = Cast<AEnemyChar>(inPawn);

	MyChar->GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
	MyChar->GetCharacterMovement()->DefaultLandMovementMode = MOVE_NavWalking;
	GameState = Cast<ABDGameState>(GetWorld()->GetGameState());
	UEnemyAIAction* Action = nullptr;
	Action = NewObject<UEnemyAttackAIAction>();
	if (Action != nullptr)
	{
		Action->Initialise(this);
		Actions.Add(Action);
	}

	UEnemyAIAction* Action2 = nullptr;
	Action2 = NewObject<UEnemyMoveAIAction>();
	if (Action != nullptr)
	{
		Action2->Initialise(this);
		Actions.Add(Action2);
	}



	if (Role == ROLE_Authority)
	{
		WhatDo();
	}
}



void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
}


void AEnemyAIController::OnMoveCompleted(const FAIRequestID RequestID, const FPathFollowingResult &Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	OnMovementComplete.Broadcast(Result.Code);
	UE_LOG(EnemyAIController, Log, TEXT("Walker %s completed move at (%s)"),
		*GetPawn()->GetName(),
		*GetPawn()->GetActorLocation().ToString());
}

void AEnemyAIController::SetAggroTarget()
{

}

void AEnemyAIController::WhatDo()
{
	if (!GetPawn()->IsValidLowLevelFast() || GetPawn()->IsActorBeingDestroyed() || CurrentAction != nullptr && CurrentAction->Executing && !CurrentAction->SafeToAbort)
	{
		return;
	}
	for (int i = 0; i < Actions.Num(); i++)
	{
		/*if (CurrentAction != nullptr && CurrentAction == Actions[i])
		{
			continue;
		}*/
		if (Actions[i]->ShouldActivate == true)
		{
			if (CurrentAction)
			{
				CurrentAction->Abort();
			}
			Actions[i]->Activate();
			CurrentAction = Actions[i];
			break;
		}
	}
	
}
