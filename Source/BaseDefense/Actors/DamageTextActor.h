// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Color.h"
#include "DamageTextActor.generated.h"

UCLASS()
class BASEDEFENSE_API ADamageTextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageTextActor();

protected:
	
	UFUNCTION()
	void AnimationFinished();
public:	
	void Initialise(FString AString, FColor AColor);
public:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* WidgetComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent = nullptr;

	FString String = "";
	FColor Color = FColor::Black;
	class UDamageText* DamageText = nullptr;
};
