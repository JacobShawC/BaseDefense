// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerActions/PlayerAction.h"
#include "UpgradeAction.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UUpgradeAction : public UPlayerAction
{
	GENERATED_BODY()
	
public:
	bool UpgradeBuilding(class ABuilding* ABuilding);
	void CancelAction() override;
	class ABuilding* CurrentBuilding = nullptr;
	FBuildingData Data;

protected:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	bool Upgrading = false;
private:
	class UBDGameInstance* GameInstance = nullptr;
	UPROPERTY()
		class APlayerChar* PlayerChar = nullptr;

	UPROPERTY()
		class ABDPlayerState* PlayerState = nullptr;

	UPROPERTY()
		class APlayerController* Controller = nullptr;
};
