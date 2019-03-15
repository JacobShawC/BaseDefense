// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructLibrary.h"
#include "Projectile.generated.h"

UCLASS()
class BASEDEFENSE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ReachedTarget();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Initialise(AActor* AnOwner, AActor* ATarget, FAttack AnAttack);

public:

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	AActor* OwningActor = nullptr;

	UPROPERTY(VisibleAnywhere)
	AActor* Target = nullptr;

	UPROPERTY(VisibleAnywhere)
	FAttack Attack;

};
