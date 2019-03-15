// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StructLibrary.h"
#include "EnemyChar.generated.h"

UCLASS()
class BASEDEFENSE_API AEnemyChar : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AEnemyChar();
	void Initialise(EEnemy AnEnemy);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** don't export collisions for navigation */
	virtual bool IsComponentRelevantForNavigation(UActorComponent* Component) const override { return false; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void OnKilled();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnDamageText(float ABounty);

	void SpawnDamageText(FString AText, FColor AColor);
	UFUNCTION()
	void OnMouseEnter(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
	void OnMouseLeave(UPrimitiveComponent * TouchedComponent);

	UFUNCTION()
	void OnRep_SetFloatingHeight();
public:


	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* FloatingWidget = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* OverlapCapsule = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	class UFloatingEnemyInfo* FloatingInfo = nullptr;

	class UBDGameInstance* GameInstance = nullptr;
	UPROPERTY(VisibleAnywhere)
	FEnemyData EnemyData;
	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComponent = nullptr;
	EEnemy Enemy = EEnemy::None;
	float Health = 100;


	UPROPERTY(ReplicatedUsing = OnRep_SetFloatingHeight)
	float FloatingHeight = 0;
};
