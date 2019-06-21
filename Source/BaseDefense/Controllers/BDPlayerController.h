// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <GameFramework/PlayerInput.h>
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
public:
	UFUNCTION(Server, Unreliable, WithValidation)
		void ServerUpdatePositions(FVector APawnPosition, FVector AMousePosition);

	void GetKeysForAction(FName AnActionName, TArray<FInputActionKeyMapping>& ABindings);


	UFUNCTION(Client, Reliable, WithValidation, Category = "Things")
		void ClearHUDWidgets();


	UPROPERTY()
	class UGUI* GUIWidget = nullptr;

	UPROPERTY()
	TScriptInterface<class IInteraction> CurrentlySelected = nullptr;

};
