// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "StructLibrary.h"
#include "PlayerAction.h"

#include "CoreMinimal.h"
#include "RepairAction.generated.h"

/**
 * 
 */
UENUM()
enum class ERepairCancelReason : uint8
{
	None 				UMETA(DisplayName = "None"),
	Distance 			UMETA(DisplayName = "Distance"),
	Killed 				UMETA(DisplayName = "Killed"),
	Finished			UMETA(DisplayName = "Finished"),
	Cancelled			UMETA(DisplayName = "Cancelled")
};


UCLASS()
class BASEDEFENSE_API URepairAction : public UPlayerAction
{
	GENERATED_BODY()

public:

	URepairAction(const FObjectInitializer& ObjectInitializer);
	bool RepairBuilding(class ABuilding* ABuilding);
	void CancelRepair(ERepairCancelReason AReason);
	void CancelAction() override;
protected:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

	float Color = 10;

	bool Repairing;
	class ABuilding* Building = nullptr;
	class UHealthComponent* HealthComponent = nullptr;
private:
	class UBDGameInstance* GameInstance = nullptr;
	UPROPERTY()
	class APlayerChar* PlayerChar = nullptr;

	UPROPERTY()
	class ABDPlayerState* PlayerState = nullptr;

	UPROPERTY()
	class APlayerController* Controller = nullptr;

	FPlayerData PlayerData;
};
