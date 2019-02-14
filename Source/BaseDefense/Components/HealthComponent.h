// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckHealthFull, bool, AHealthFull);



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
	void OnRep_LastDamage();
	UFUNCTION()
	void OnRep_LastHeal();
	UFUNCTION()
	void OnRep_SetMaxHealth();

	void Initialise(float AMaxHealth);
	void TakeDamage(float ADamage);
	float Heal(float AHeal);

	void CheckFull();



	virtual void BeginPlay() override;
	void SpawnDamageText(FString AText, FColor AColor);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UPROPERTY()
	FCheckHealthFull HealthFullDelegate;


	bool LastKnownFull = true;

	UPROPERTY(ReplicatedUsing = OnRep_SetMaxHealth)
	float MaxHealth = 0;

	UPROPERTY(ReplicatedUsing = OnRep_SetHealth)
	float Health = 0;
	
	UPROPERTY(ReplicatedUsing = OnRep_LastDamage)
	float LastDamage = 0;

	UPROPERTY(ReplicatedUsing = OnRep_LastHeal)
	float LastHeal = 0;

	float LastKnownHealth = 0;


	UPROPERTY()
	class UFloatingInfo* FloatingWidget = nullptr;

};
