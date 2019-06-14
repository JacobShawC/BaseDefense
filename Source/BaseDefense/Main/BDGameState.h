// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MiniMapCapture.h"
#include "StructLibrary.h"
//#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\AnimGraphRuntime\Public\KismetAnimationLibrary.h"
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
public:

	UFUNCTION()
	void OnRep_LevelRewards();
	UFUNCTION()
	void OnRep_LevelSaves();
	UFUNCTION()
	void OnRep_CurrentState();
	UFUNCTION()
	void OnRep_SelectedLevel();
	UFUNCTION()
	void OnRep_SelectedLevelDifficulty();

	UFUNCTION(Exec)
	void SetLevelRewards(int ALevelRewards);

	void StartGame();

	float AddMoney(float AMoney);

	void SetCurrentState(EGameState AState);

	virtual void BeginPlay() override;

	void AddBuilding(TWeakObjectPtr<class ABuilding> ABuilding);
	void AddEnemyCharacter(TWeakObjectPtr<class AEnemyChar> AnEnemyChar);
	void SetMapProperties(FVector APosition, float AWidth);

	void RefreshLevelRewards();

	EGameState GetCurrentState();
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
	FVariableUpdated CurrentStateUpdated;
	FVariableUpdated SelectedLevelUpdated;
	FVariableUpdated SelectedLevelDifficultyUpdated;

	UPROPERTY(ReplicatedUsing = OnRep_SelectedLevel)	ELevel SelectedLevel = ELevel::None;

	class ABaseLevel* LevelActor = nullptr;
	
	UPROPERTY(ReplicatedUsing = OnRep_SelectedLevelDifficulty)	ELevelDifficulty SelectedLevelDifficulty = ELevelDifficulty::None;

	UPROPERTY(ReplicatedUsing=OnRep_LevelRewards)	int LevelRewards = 0;

	UPROPERTY(ReplicatedUsing = OnRep_LevelSaves)	TMap<ELevel, FLevelSave> LevelSaves;

	int NumberOfPlayers = 1;

	int WorldGridSize = 256;

private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)	EGameState CurrentState = EGameState::PreGame;

protected:

};