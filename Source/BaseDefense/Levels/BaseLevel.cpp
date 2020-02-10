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
	if (GetLocalRole() == ROLE_Authority)
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

void ABaseLevel::SpawnWave(EEnemy AnEnemyType, int ABoxNumber, float AFrequency, float AStartSeconds, float AEndSeconds)
{
	if (AStartSeconds <= AEndSeconds)
	{
		if (SpawnBoxes.Num() >= ABoxNumber)
		{
			float TotalTime = AEndSeconds - AStartSeconds;
			for (float Time = 0; Time <= AEndSeconds; Time += AFrequency)
			{
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(SpawnBoxes[ABoxNumber - 1], FName("SpawnEnemy"), AnEnemyType);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Time, false);
			}
			LogEvent(ELevelEvent::Wave, AnEnemyType, ABoxNumber, AFrequency, AStartSeconds, AEndSeconds);
		}
	}
}

void ABaseLevel::LogEvent(ELevelEvent AEventType, EEnemy AnEnemyType, int ABoxNumber, float AFrequency, float AStartSeconds, float AEndSeconds)
{
	FLevelEvent LevelEvent;
	LevelEvent.Event = AEventType;
	LevelEvent.StartSeconds = AStartSeconds;
	LevelEvent.EndSeconds = AEndSeconds;
	Events.Add(LevelEvent);
}

