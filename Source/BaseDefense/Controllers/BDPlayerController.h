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
	/** Reports the current server time to clients in response
	  * to ServerRequestServerTime */
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientReportServerTime(float requestWorldTime, float serverTime);

	/** Requests current server time so accurate lag
	  * compensation can be performed in ClientReportServerTime
	  * based on the round-trip duration */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestServerTime(APlayerController* requester, float requestWorldTime);


public:

	/** Returns the network-synced time from the server.
	* Corresponds to GetWorld()->GetTimeSeconds()
	* on the server. This doesn't actually make a network
	* request; it just returns the cached, locally-simulated
	* and lag-corrected ServerTime value which was synced
	* with the server at the time of this PlayerController's
	* last restart. */
	virtual float GetServerTime() { return ServerTime; }

	virtual void ReceivedPlayer() override;


	

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
	
	float ServerTime = 0.0f;


	UPROPERTY()
	TScriptInterface<class IInteraction> CurrentlySelected = nullptr;

};
