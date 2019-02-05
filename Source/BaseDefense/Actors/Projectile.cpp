// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "HealthComponent.h"
#include "UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AProjectile::AProjectile()
{
	bReplicates = true;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetIsReplicated(true);
	PrimaryActorTick.bStartWithTickEnabled = true;
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Initialise(AActor* AnActor, FAttack AnAttack)
{
	if (!AnActor)
	{
		Destroy();
	}
	Target = AnActor;
	Attack = AnAttack;
	FRotator LookAtRotat = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), AnActor->GetActorLocation());
	MeshComponent->SetStaticMesh(AnAttack.Projectile.Mesh);

}

void AProjectile::ReachedTarget()
{
	SetActorTickEnabled(false);
	if (Role == ROLE_Authority)
	{
		if (Target != nullptr)
		{
			UHealthComponent* HealthComponent = nullptr;
			HealthComponent = Cast<UHealthComponent>(Target->GetComponentByClass(UHealthComponent::StaticClass()));
			if (HealthComponent)
			{
				HealthComponent->TakeDamage(Attack.Damage);
			}
		}
	}
	Destroy();

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target != nullptr)
	{
		FVector Location = this->GetActorLocation();
		FRotator LookAtRotat = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Target->GetActorLocation());
		this->SetActorRotation(LookAtRotat);
		float Distance = FVector::Dist(Location, Target->GetActorLocation());
		float Travel = Attack.Projectile.Speed * DeltaTime;
		if (Distance <= Travel)
		{
			ReachedTarget();
		}
		else
		{
			FVector NewLocation = this->GetActorLocation() + LookAtRotat.Vector() * Travel;
			this->SetActorLocation(NewLocation);
		}

	
	}

}

