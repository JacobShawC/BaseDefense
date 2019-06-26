// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <GameFramework/PlayerInput.h>
#include "StructLibrary.h"
#include "BDPlayerController.generated.h"

/**
 *
 */
UCLASS()
class BASEDEFENSE_API ABDPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	ABDPlayerController(const FObjectInitializer& ObjectInitializer);
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

	void MakeGhost();

	void UpdateCommands();


public:
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerUpdatePositions(FVector APawnPosition, FVector AMousePosition);

	void GetKeysForAction(FName AnActionName, TArray<FInputActionKeyMapping>& ABindings);

	void SelectHotbar();

	void SetSelectedBuilding(EBuilding ABuilding);

	void CancelAction();


	UFUNCTION(Client, Reliable, WithValidation, Category = "Things")
	void ClearHUDWidgets();

	
	UPROPERTY()
	class UGUI* GUIWidget = nullptr;

	class ABuildingGhost* BuildingGhost = nullptr;

	EBuilding SelectedBuilding = EBuilding::None;
	

	UPROPERTY()
	TScriptInterface<class IInteraction> CurrentlySelected = nullptr;

};
