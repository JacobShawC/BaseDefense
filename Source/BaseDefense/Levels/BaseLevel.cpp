// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseLevel.h"
#include "Public/TimerManager.h"
#include "Engine/World.h"
#include "SpawnBox.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABaseLevel::ABaseLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseLevel::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnBox::StaticClass(), FoundActors);

		while (FoundActors.Num() > 0)
		{

			ASpawnBox* LowestActor = nullptr;

			for (AActor* AnActor : FoundActors)
			{
				ASpawnBox* SpawnActor = Cast<ASpawnBox>(AnActor);

				if (LowestActor != nullptr)
				{
					if (SpawnActor->ID < LowestActor->ID)
					{
						LowestActor = SpawnActor;
					}
				}
				else
				{
					LowestActor = SpawnActor;
				}

			}
			SpawnBoxes.Add(LowestActor);
			FoundActors.Remove(LowestActor);
		}

		RunLevel();
	}
	
}

// Called every frame
void ABaseLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseLevel::RunLevel()
{

}

void ABaseLevel::SpawnWave(EEnemy AnEnemyType, int ABoxNumber, int ANumber, float AFrequency, float AStartTime)
{
	if (SpawnBoxes.Num() >= ABoxNumber)
	{
		float BeginingTime = AStartTime;
		for (int i = 0; i < ANumber; i++)
		{
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(SpawnBoxes[ABoxNumber - 1], FName("SpawnEnemy"), AnEnemyType);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, BeginingTime, false);
			BeginingTime += AFrequency;
		}
		LogEvent(ELevelEvent::Wave, AStartTime, AStartTime + ANumber * AFrequency);
	
	}
}

void ABaseLevel::LogEvent(ELevelEvent AEventType, float AStartTime, float ADuration)
{
	FLevelEvent LevelEvent;
	LevelEvent.Event = AEventType;
	LevelEvent.StartTime = AStartTime;
	LevelEvent.Duration = ADuration;
	Events.Add(LevelEvent);
}

