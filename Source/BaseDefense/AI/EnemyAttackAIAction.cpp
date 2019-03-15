// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyAttackAIAction.h"
#include "EnemyChar.h"
#include "Engine/World.h"
#include "BDGameInstance.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Public/TimerManager.h"
#include "Public/DrawDebugHelpers.h"

void UEnemyAttackAIAction::Initialise(AEnemyAIController* AnOwner)
{
	Super::Initialise(AnOwner);

	if (Char)
	{
		RangeCapsule = NewObject<UCapsuleComponent>(AnOwner, TEXT("RangeCapsule"));

		RangeCapsule->SetCollisionProfileName("EnemyRange");
		RangeCapsule->SetGenerateOverlapEvents(true);
		RangeCapsule->SetCapsuleSize(EnemyData->Attack.Range, 50, true);
		RangeCapsule->RegisterComponent();
		RangeCapsule->AttachToComponent(Char->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);

		RangeCapsule->bVisible = true;
		if (RangeCapsule)
		{
			RangeCapsule->OnComponentBeginOverlap.AddDynamic(this, &UEnemyAttackAIAction::OnRangeOverlap);
			RangeCapsule->OnComponentEndOverlap.AddDynamic(this, &UEnemyAttackAIAction::OnOverlapEnd);
		}
	}
	SafeToAbort = false;

	SetCurrentTarget();

}

void UEnemyAttackAIAction::Activate()
{
	UHealthComponent* HealthComponent = nullptr;

	float Distance = 0;

	if (CurrentTarget != nullptr && !CurrentTarget->IsPendingKill())
	{
		Distance = FVector::Dist(Char->GetActorLocation(), CurrentTarget->GetActorLocation());
		Distance -= 50;
	}
	
	//Check if target is still in range and exists
	if (!(CurrentTarget != nullptr && !CurrentTarget->IsPendingKill() && Distance <= EnemyData->Attack.Range))
	{
		CurrentTarget = nullptr;
		SetCurrentTarget();
	}
	if (CurrentTarget != nullptr && CurrentTarget->IsValidLowLevelFast())
	{
		HealthComponent = Cast<UHealthComponent>(CurrentTarget->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComponent)
		{
			AIController->StopMovement();
			HealthComponent->TakeDamage(AIController->GetOwner(), EnemyData->Attack.Damage);

			DrawDebugLine(Char->GetWorld(), Char->GetActorLocation(), CurrentTarget->GetActorLocation(), FColor::Red, true, 0.5f);

			// attack target
			FTimerHandle FuzeTimerHandle;
			Char->GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UEnemyAttackAIAction::FinishedAttack, EnemyData->Attack.ReloadTime, false);
			Executing = true;
			return;
		}
	}

	//this should never happen
	CurrentTarget = nullptr;
	ShouldActivate = false;

	Executing = false;
	if (AIController->CurrentAction == this)
	{
		AIController->CurrentAction = nullptr;
	}

	AIController->WhatDo();
	
}

void UEnemyAttackAIAction::SetShouldActivate()
{
	
}

void UEnemyAttackAIAction::SetCurrentTarget()
{
	//dont do anything if we're executing
	if (!Executing)
	{
		//find the target that conforms to the attack rule
		TArray<AActor*> Actors;
		if (RangeCapsule != nullptr && RangeCapsule)
		{
			RangeCapsule->GetOverlappingActors(Actors);
		}
		EAttackRule AttackRule = EnemyData->Attack.AttackRule;
		//find closest or furthest target
		if (AttackRule == EAttackRule::Closest || AttackRule == EAttackRule::Furthest)
		{
			AActor* Target = nullptr;

			if (Actors.Num() >= 0)
			{
				bool First = true;
				float Dist = 0;
				FVector CharLocation = Char->GetActorLocation();
				for (int i = 0; i < Actors.Num(); i++)
				{
					if (!Actors[i]->IsValidLowLevel() || Actors[i]->IsPendingKill())
					{
						continue;
					}
					float Distance = FVector::Dist(CharLocation, Actors[i]->GetActorLocation());
					if (AttackRule == EAttackRule::Closest)
					{
						if ((First || Distance < Dist) && Actors[i]->GetComponentByClass(UHealthComponent::StaticClass()))
						{
							if (First)
							{
								First = false;
							}
							Dist = Distance;
							Target = Actors[i];
						}
					}
					else
					{
						if ((First || Distance > Dist) && Actors[i]->GetComponentByClass(UHealthComponent::StaticClass()))
						{
							if (First)
							{
								First = false;
							}
							Dist = Distance;
							Target = Actors[i];
						}
					}
				}
			}
			if (Target)
			{
				CurrentTarget = Target;
				ShouldActivate = true;

			}
			else
			{
				CurrentTarget = nullptr;
				ShouldActivate = false;
			}
		}
		//find highest or lowest health
		else if (AttackRule == EAttackRule::HighestHealth || AttackRule == EAttackRule::LowestHealth)
		{
			AActor* Target = nullptr;

			if (Actors.Num() >= 0)
			{
				bool First = true;
				float Health = 0;
				for (int i = 0; i < Actors.Num(); i++)
				{
					if (!Actors[i]->IsValidLowLevel() || Actors[i]->IsPendingKill())
					{
						continue;
					}
					UHealthComponent* HealthComponent = nullptr;
					HealthComponent = Cast<UHealthComponent>(Actors[i]->GetComponentByClass(UHealthComponent::StaticClass()));
					if (HealthComponent == nullptr)
					{
						continue;
					}
					if (AttackRule == EAttackRule::HighestHealth)
					{
						if (First || HealthComponent->Health > Health)
						{
							if (First)
							{
								First = false;
							}
							Health = HealthComponent->Health;
							Target = Actors[i];
						}
					}
					else
					{
						if (First || HealthComponent->Health < Health)
						{
							if (First)
							{
								First = false;
							}
							Health = HealthComponent->Health;
							Target = Actors[i];
						}
					}
				}
			}
			if (Target)
			{
				CurrentTarget = Target;
				ShouldActivate = true;

			}
			else
			{
				CurrentTarget = nullptr;
				ShouldActivate = false;
			}
		}
	}
	else
	{
		CurrentTarget = nullptr;
		ShouldActivate = false;
	}
}

void UEnemyAttackAIAction::Abort()
{
}

void UEnemyAttackAIAction::OnRangeOverlap(UPrimitiveComponent* AComponent, AActor* AnActor, class UPrimitiveComponent* OtherComponent, int32 ABodyIndex, bool bFromSweep, const FHitResult& ASweepResult)
{
	Overlapping++;
	ShouldActivate = true;
	//SetCurrentTarget();
	
	if (!Executing)
	{
		AIController->WhatDo();
	}
}

void UEnemyAttackAIAction::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*Overlapping--;
	if (Overlapping < 0)
	{
		Overlapping = 0;
	}

	if (Overlapping > 0)
	{
		ShouldActivate = true;
	}
	else
	{
		ShouldActivate = false;
	}*/
}

void UEnemyAttackAIAction::FinishedAttack()
{
	Executing = false;
	if (AIController->CurrentAction == this)
	{
		AIController->CurrentAction = nullptr;
	}

	AIController->WhatDo();


}
