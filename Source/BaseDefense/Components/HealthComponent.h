// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckHealthFull, bool, AHealthFull);

//DECLARE_MULTICAST_DELEGATE_TwoParams(FBeingAttacked, AActor*, AnAttackingActor, float, ADamage);
DECLARE_MULTICAST_DELEGATE_TwoParams(FBeingAttacked, AActor*, float);



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
	void TakeDamage(AActor* AnActor, float ADamage);
	float Heal(float AHeal);

	void ChangeMaxHealth(float AMaxHealth, bool ChangesCurrentHealth);

	void CheckFull();

	FBeingAttacked OnAttacked;

	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnDamageText(const FString& AText, FColor AColor);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UPROPERTY()
	FCheckHealthFull HealthFullDelegate;


	bool LastKnownFull = true;

	UPROPERTY(ReplicatedUsing = OnRep_SetMaxHealth)
	float MaxHealth = 0;

	UPROPERTY(ReplicatedUsing = OnRep_SetHealth)
	float Health = 0;



	UPROPERTY()
	class UFloatingInfo* FloatingWidget = nullptr;

};
