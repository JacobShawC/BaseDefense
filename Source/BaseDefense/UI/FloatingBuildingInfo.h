// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FloatingInfo.h"
#include "FloatingBuildingInfo.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UFloatingBuildingInfo : public UFloatingInfo
{
	GENERATED_BODY()
	
public:

	void SetMaxConstruction(float AMaxConstruction);
	void SetConstruction(float AConstruction);
	void SetConstructionText(float AConstruction);
	void SetConstructionProgressBar(float AConstruction);
	void SetConstructionVisibility(bool AVisiblity);
	void SetConstructionTextVisibility(bool AVisiblity);
	void SetConstructionProgressBarVisibility(bool AVisiblity);

public:



	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConstructionText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ConstructionProgressBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* ConstructionSizeBox = nullptr;


	float MaxConstruction = 100;
	float Construction = 0;
};
