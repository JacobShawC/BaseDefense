// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuController.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API AMenuController : public APlayerController
{
	GENERATED_BODY()

	AMenuController(const FObjectInitializer& ObjectInitializer);
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

public:
	void SetMenuLoading();

	UFUNCTION(Client, Reliable, Category = "Things")
		void ClearHUDWidgets();
	virtual void ClearHUDWidgets_Implementation();
public:
	class UMainMenu* Menu = nullptr;

private:
	class UBDGameInstance* GameInstance = nullptr;
	class UMainMenu* MainMenuUserWidget = nullptr;

};
