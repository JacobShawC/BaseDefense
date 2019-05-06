// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "StructLibrary.h"
#include "BDPlayerController.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(BDPlayerController, Log, All);

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API ABDPlayerController : public APlayerController
{
	GENERATED_BODY()
	

protected:
	//ABDPlayerController();
	ABDPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	

	FBuildingLocationInfo GetLocationInfo(FHitResult ATrace);

	virtual void PlayerTick(float DeltaTime) override;

	FHitResult DoTraceMouse(TArray<AActor*> ActorsToIgnore, ECollisionChannel AChannel);
	FHitResult RoundedMouseTrace(ECollisionChannel AChannel);

	FHitResult DoSingleTrace(FVector Start, FVector End, TArray<AActor*> ActorsToIgnore, ECollisionChannel AChannel);
	void MakeGhost();

	void UpdateCommands();

	void UpdateGhost(bool AReachable, bool ABuildable);
public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerConstructBuilding(EBuilding ABuildingEnum, FVector APosition);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSelectLoadout(EBuildingUpgrade AnUpgrade, bool AddOrRemove);



	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpgradeBuilding(class ABuilding* ABuilding);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCancelAction();

	void GetKeysForAction(FName AnActionName, TArray<FInputActionKeyMapping>& Bindings);

	bool ChangePlayerMoney(float AMoney);
	bool IsSelectedValid();
	void SelectHotbar(int ASlot);
	bool GetActionHeld(FName AText);

	void SetCurrentlySelected(TScriptInterface<class IInteraction> ACurrentlySelected);
	//void TryBuildBuildingFromGhost();


	template<int32 ASlot>
	void SelectHotbar()
	{
		SelectHotbar(ASlot);
	}

	void ScrollHotbar(bool UpOrDown);


	template<bool UpOrDown>
	void ScrollHotbar()
	{
		ScrollHotbar(UpOrDown);
	}

	void SelectPressed();

	void SelectReleased();

	void SelectAltPressed();

	void SelectAltReleased();

	void UsePressed();

	void UseReleased();

	void UseAltPressed();

	void UseAltReleased();

	void RepairPressed();

	void RepairReleased();

public:
	UPROPERTY()
	TScriptInterface<class IInteraction> CurrentlySelected = nullptr;
	//class IInteraction* CurrentlySelected = nullptr;

	UPROPERTY()
	class UGUI* GUIWidget = nullptr;
	UPROPERTY()
	class ABuildingGhost* BuildingGhost = nullptr;



protected:
	class UWorld* World = nullptr;
	class UBDGameInstance* GameInstance = nullptr;



};
