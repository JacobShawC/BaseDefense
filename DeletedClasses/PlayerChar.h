// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StructLibrary.h"
#include "Death.h"
#include "PlayerChar.generated.h"

UCLASS()
class BASEDEFENSE_API APlayerChar : public ACharacter, public IDeath
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool Kill() override;

	class AActor* CurrentlyHovering = nullptr;

	class UDecalComponent* RangeDecal = nullptr;

	FPlayerData PlayerData;

public:
	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* PlayerCapsule = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* FloatingWidget = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComponent = nullptr;

	UPROPERTY()
	class UConstructAction* ConstructAction = nullptr;
	UPROPERTY()
	class URepairAction* RepairAction = nullptr;
	UPROPERTY()
	class UUpgradeAction* UpgradeAction = nullptr;

	UPROPERTY(Replicated)
	class UPlayerAction* CurrentAction = nullptr;

	UPROPERTY(Replicated)
	class AActor* ActionTarget = nullptr;
	UPROPERTY(Replicated)
	bool FollowAction = false;

protected:
	class UBDGameInstance* GameInstance = nullptr;
};
