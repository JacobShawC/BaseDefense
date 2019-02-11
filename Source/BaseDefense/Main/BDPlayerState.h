// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Main/StructLibrary.h"
#include "BDPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API ABDPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	void SetUpTestData();

	bool ChangePlayerMoney(float AMoney);
	bool CanChangePlayerMoney(float AMoney);
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	UFUNCTION()
	void OnRep_Money();
public:
	UPROPERTY(Replicated)
	FPlayerData PlayerData;

	UPROPERTY(ReplicatedUsing = OnRep_Money)
	float Money = 500;

};
