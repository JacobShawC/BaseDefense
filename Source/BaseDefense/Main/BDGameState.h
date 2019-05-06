// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MiniMapCapture.h"
#include "BDGameState.generated.h"

DECLARE_MULTICAST_DELEGATE(FVariableUpdated);

/**
 *	
 */
UCLASS()
class BASEDEFENSE_API ABDGameState : public AGameState
{
	GENERATED_BODY()

protected:
	void GetMiniMap();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	UFUNCTION()
	void OnRep_LevelRewards();
	UFUNCTION()
	void OnRep_LevelSaves();	
	UFUNCTION()
	void OnRep_GameState();
public:

	float AddMoney(float AMoney);

	virtual void BeginPlay() override;

	void AddBuilding(TWeakObjectPtr<class ABuilding> ABuilding);
	void AddEnemyCharacter(TWeakObjectPtr<class AEnemyChar> AnEnemyChar);
	void SetMapProperties(FVector APosition, float AWidth);

	void RefreshLevelRewards();
public:
	UPROPERTY()
	TArray<TWeakObjectPtr<class ABuilding>> Buildings;

	UPROPERTY()
	TArray<TWeakObjectPtr<class AEnemyChar>> EnemyCharacters;


	TArray<class APlayerChar*> GetPlayerPawns();

	TWeakObjectPtr<AMiniMapCapture> MiniMapCamera;

	FBox WorldBounds;

	float OrthoWidth = 0;
	FVector2D MapCapturePosition;

	class UMiniMap* MiniMap = nullptr;

	FVariableUpdated LevelRewardsUpdated;
	FVariableUpdated LevelSavesUpdated;
	FVariableUpdated GameStateUpdated;
	UPROPERTY(ReplicatedUsing=OnRep_LevelRewards)	int LevelRewards = 0;

	UPROPERTY(ReplicatedUsing = OnRep_LevelSaves)	TMap<ELevel, FLevelSave> LevelSaves;

	UPROPERTY(ReplicatedUsing = OnRep_GameState)	EGameState GameState = EGameState::PreGame;

protected:
	class UBDGameInstance* GameInstance = nullptr;

};