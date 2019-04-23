// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MiniMapCapture.h"
#include "BDGameState.generated.h"

/**
 *	
 */
UCLASS()
class BASEDEFENSE_API ABDGameState : public AGameState
{
	GENERATED_BODY()

protected:
	void GetMiniMap();

public:

	float AddMoney(float AMoney);

	virtual void BeginPlay() override;

	void AddBuilding(TWeakObjectPtr<class ABuilding> ABuilding);
	void AddEnemyCharacter(TWeakObjectPtr<class AEnemyChar> AnEnemyChar);
	void SetMapProperties(FVector APosition, float AWidth);
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



};