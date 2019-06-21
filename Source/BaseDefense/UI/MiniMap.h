// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMap.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FMiniMapClicked, FVector2D);


/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UMiniMap : public UUserWidget
{
	GENERATED_BODY()
	
private:

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	FVector2D GetCanvasLocation(FVector ALocation);

	FVector GetWorldLocationFromCanvas(FVector2D ALocation);
public:
	void SetUp();

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


	void MiniMapClicked(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void Refresh();

	UFUNCTION()
	void SetMiniMap();

	void RefreshMiniMapTexture();

	void AddBuilding(TWeakObjectPtr<class ABuilding> ABuildingInput);
	void AddEnemyCharacter(TWeakObjectPtr<class AEnemyChar> AnEnemyChar);

	FMiniMapClicked OnMiniMapClicked;

public:
	UPROPERTY()
	TMap<TWeakObjectPtr<class ABuilding>, class UImage*> Buildings;

	UPROPERTY()
	TMap<TWeakObjectPtr<class AEnemyChar>, class UImage*> EnemyCharacters;

	UPROPERTY()
	TMap<TWeakObjectPtr<class APawn>, class UImage*> Players;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* MiniMapPanel = nullptr; 
	
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* BuildingsPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* EnemiesPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* PlayersPanel = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* MiniMapTexture = nullptr;

	float MapWidth = 256000;
	FVector2D MapPosition;

	float BuildingSize = 10;
	float EnemySize = 10;
	float PlayerSize = 15;

	float WidgetSize = 400;
};
