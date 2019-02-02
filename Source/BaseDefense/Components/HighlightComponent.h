// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASEDEFENSE_API UHighlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHighlightComponent();

protected:
	
public:	
	void Initialise(class UMeshComponent* AMesh, int AStencilValue);
	void SetHighlight(int AnInt);
	void SetHighlightVisible(bool AVisible);
};
