
#pragma once

#include "CoreMinimal.h"
#include "PlayerActions/PlayerAction.h"
#include "StructLibrary.h"
#include "ConstructAction.generated.h"

UENUM()
enum class EConstructionCancelReason : uint8
{
	None 				UMETA(DisplayName = "None"),
	Distance 			UMETA(DisplayName = "Distance"),
	Killed 			UMETA(DisplayName = "Killed"),
	Finished			UMETA(DisplayName = "Finished"),
	Cancelled			UMETA(DisplayName = "Cancelled")
};
/**
 *
 */
UCLASS()
class BASEDEFENSE_API UConstructAction : public UPlayerAction
{
	GENERATED_BODY()

public:
	UConstructAction(const FObjectInitializer& ObjectInitializer);
	bool CanConstructBuilding();
	bool ConstructBuilding(FBuildingData AData, FVector APosition);
	void CancelConstruction(EConstructionCancelReason AReason);
	void CancelAction() override;
	class ABuilding* CurrentBuilding = nullptr;
	FBuildingData Data;

protected:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	bool Constructing = false;
private:
	class UBDGameInstance* GameInstance = nullptr;
	UPROPERTY()
	class APlayerChar* PlayerChar = nullptr;

	UPROPERTY()
	class ABDPlayerState* PlayerState = nullptr;

	UPROPERTY()
	class APlayerController* Controller = nullptr;
};


