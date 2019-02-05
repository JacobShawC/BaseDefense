// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASEDEFENSE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION()
	void OnRep_SetHealth();
	UFUNCTION()
	void OnRep_SetMaxHealth();

	void Initialise(float AMaxHealth);
	void TakeDamage(float ADamage);
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_SetMaxHealth)
	int MaxHealth = 0;

	UPROPERTY(ReplicatedUsing = OnRep_SetHealth)
	int Health = 0;
	
	int LastKnownHealth = 0;


	class UFloatingInfo* FloatingWidget = nullptr;

};
