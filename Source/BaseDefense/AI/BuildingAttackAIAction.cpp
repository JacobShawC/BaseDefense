// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingAttackAIAction.h"
#include "Engine/World.h"
#include "BDGameInstance.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Public/TimerManager.h"
#include "Building.h"
#include "Components/PrimitiveComponent.h"
#include "Projectile.h"
#include "Public/DrawDebugHelpers.h"
void UBuildingAttackAIAction::Initialise(ABuilding* ABuilding)
{
	Super::Initialise(ABuilding);

	if (Building)
	{
		RangeCapsule = NewObject<UCapsuleComponent>(Building, TEXT("RangeCapsule"));

		RangeCapsule->SetCollisionProfileName("BuildingRange");
		RangeCapsule->SetGenerateOverlapEvents(true);
		RangeCapsule->SetCapsuleSize(BuildingData->Attack.Range, 50, true);
		RangeCapsule->RegisterComponent();
		RangeCapsule->AttachToComponent(Building->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);

		RangeCapsule->bVisible = true;
		if (RangeCapsule)
		{
			RangeCapsule->OnComponentBeginOverlap.AddDynamic(this, &UBuildingAttackAIAction::OnRangeOverlap);
			RangeCapsule->OnComponentEndOverlap.AddDynamic(this, &UBuildingAttackAIAction::OnOverlapEnd);
		}
	}
	SafeToAbort = false;
	SetCurrentTarget();

}

void UBuildingAttackAIAction::Activate()
{
	UHealthComponent* HealthComponent = nullptr;

	float Distance = 0;

	if (CurrentTarget && !CurrentTarget->IsPendingKill())
	{
		Distance = FVector::Dist(Building->GetActorLocation(), CurrentTarget->GetActorLocation());
		Distance -= 75;
	}

	//Check if target is still in range and exists
	if (!(CurrentTarget != nullptr && CurrentTarget->IsValidLowLevel() && !CurrentTarget->IsPendingKill() && Distance <= BuildingData->Attack.Range))
	{
		CurrentTarget = nullptr;
		SetCurrentTarget();
	}
	if (CurrentTarget != nullptr)
	{
		HealthComponent = Cast<UHealthComponent>(CurrentTarget->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComponent)
		{
			//HealthComponent->TakeDamage(BuildingData->Attack.Damage);
			DrawDebugLine(Building->GetWorld(), Building->GetActorLocation(), CurrentTarget->GetActorLocation(), FColor::Green, true, 0.5f);
			AProjectile* Projectile = Building->GetWorld()->SpawnActor<AProjectile>(Building->GetActorLocation(), FRotator(0.0f));
			Projectile->Initialise(Building, CurrentTarget, BuildingData->Attack);
			// attack target
			FTimerHandle FuzeTimerHandle;
			Building->GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UBuildingAttackAIAction::FinishedAttack, BuildingData->Attack.ReloadTime, false);
			Executing = true;
			return;
		}
	}

	//this should never happen
	CurrentTarget = nullptr;
	ShouldActivate = false;

	Executing = false;

	Building->WhatDo();

}

void UBuildingAttackAIAction::SetShouldActivate()
{

}

void UBuildingAttackAIAction::SetCurrentTarget()
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
		EAttackRule AttackRule = BuildingData->Attack.AttackRule;
		//find closest or furthest target
		if (AttackRule == EAttackRule::Closest || AttackRule == EAttackRule::Furthest)
		{
			AActor* Target = nullptr;

			if (Actors.Num() >= 0)
			{
				bool First = true;
				float Dist = 0;
				FVector CharLocation = Building->GetActorLocation();
				for (int i = 0; i < Actors.Num(); i++)
				{
					if (Actors[i] == nullptr || !Actors[i]->IsValidLowLevel() || Actors[i]->IsPendingKill())
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
		//CurrentTarget = nullptr;
		ShouldActivate = false;
	}
}

void UBuildingAttackAIAction::Abort()
{
}

void UBuildingAttackAIAction::OnRangeOverlap(UPrimitiveComponent* AComponent, AActor* AnActor, class UPrimitiveComponent* OtherComponent, int32 ABodyIndex, bool bFromSweep, const FHitResult& ASweepResult)
{
	Overlapping++;
	ShouldActivate = true;
	//SetCurrentTarget();

	if (!Executing)
	{
		Building->WhatDo();
	}
}

void UBuildingAttackAIAction::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void UBuildingAttackAIAction::FinishedAttack()
{
	Executing = false;
	if (Building->CurrentAction == this)
	{
		Building->CurrentAction = nullptr;
	}

	Building->WhatDo();


}

