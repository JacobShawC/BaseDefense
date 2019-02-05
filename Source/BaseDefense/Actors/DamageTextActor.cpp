// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageTextActor.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/WidgetAnimation.h"
#include "Components/SceneComponent.h"
#include "DamageText.h"
#include "Blueprint/UserWidget.h"
// Sets default values
ADamageTextActor::ADamageTextActor()
{
	PrimaryActorTick.bCanEverTick = true;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent2"));
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprint'/Game/UI/DamageTextWidget.DamageTextWidget_C'"));
	WidgetComponent->SetWidgetClass(WidgetClass.Class);
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ADamageTextActor::AnimationFinished()
{
	Destroy();
}

void ADamageTextActor::Initialise(FString AString, FColor AColor)
{
	String = AString;
	Color = AColor;

	if (WidgetComponent != nullptr)
	{
		UUserWidget* UserWidget = nullptr;
		WidgetComponent->InitWidget();
		UserWidget = WidgetComponent->GetUserWidgetObject();
		DamageText = Cast<UDamageText>(UserWidget);
		if (DamageText != nullptr)
		{
			DamageText->Initialise(String, Color);
		}
	}

	if (DamageText != nullptr)
	{
		FScriptDelegate Del;
		Del.BindUFunction(this, "AnimationFinished");
		DamageText->Animation->OnAnimationFinished.AddUnique(Del);
		return;
	}
	else
	{
		Destroy();
	}
}