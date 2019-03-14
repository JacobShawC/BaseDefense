// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnBox.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "EnemyChar.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
ASpawnBox::ASpawnBox()
{
	SpawnBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
}


void ASpawnBox::SpawnEnemy(EEnemy AEnemy)
{
	AEnemyChar* Enemy = nullptr;
	FVector Extent = SpawnBoxComponent->GetScaledBoxExtent();
	FActorSpawnParameters SpawnParams;

	FVector RandomLoc = UKismetMathLibrary::RandomPointInBoundingBox(SpawnBoxComponent->GetComponentLocation(), SpawnBoxComponent->GetScaledBoxExtent());

	Enemy = Cast<AEnemyChar>(GetWorld()->SpawnActor<AEnemyChar>(AEnemyChar::StaticClass(), RandomLoc, FRotator(0.0f), SpawnParams));
	if (Enemy != nullptr)
	{
		Enemy->Initialise(AEnemy);
	}
	else
	{
		SpawnEnemy(AEnemy);
	}
}

