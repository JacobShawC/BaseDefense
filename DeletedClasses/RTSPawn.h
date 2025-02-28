// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSPawn.generated.h"

UCLASS()
class BASEDEFENSE_API ARTSPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnRep_PawnLocation();

	UFUNCTION()
	void OnRep_PointerLocation();

private:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SceneComponent = nullptr;	
	UPROPERTY(VisibleAnywhere)
		USceneComponent* MouseSceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera = nullptr;

	FVector GetCameaPanDirection();

	void SelectPressed();

	void SelectReleased();

	void SelectAltPressed();

	void SelectAltReleased();

	void MiddleMousePressed();

	void UpdatePositions();

	

	void MiddleMouseReleased();
	void ScrollHotbar(bool UpOrDown);

	template<bool UpOrDown>
	void ScrollHotbar()
	{
		ScrollHotbar(UpOrDown);
	}
	void SelectHotbar(int ASlot);

	template<int32 ASlot>
	void SelectHotbar()
	{
		SelectHotbar(ASlot);
	}

	float CamSpeed = 100;

	
public:

	UPROPERTY(ReplicatedUsing = OnRep_PawnLocation)
	FVector PawnLocation;

	UPROPERTY(ReplicatedUsing = OnRep_PointerLocation)
	FVector PointerLocation;

};
