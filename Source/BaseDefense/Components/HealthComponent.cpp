// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "FloatingInfo.h"
#include "Components/WidgetComponent.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bReplicates = true;

	// ...
}



void UHealthComponent::OnRep_SetHealth()
{
	if (FloatingWidget == nullptr)
	{
		UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetOwner()->FindComponentByClass(UWidgetComponent::StaticClass()));
		if (WidgetComponent)
		{
			FloatingWidget = Cast<UFloatingInfo>(WidgetComponent->GetUserWidgetObject());
		}
	}

	if (FloatingWidget != nullptr)
	{
		FloatingWidget->SetHealth(Health);
	}
}

void UHealthComponent::OnRep_SetMaxHealth()
{
	if (FloatingWidget == nullptr)
	{
		UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetOwner()->FindComponentByClass(UWidgetComponent::StaticClass()));
		if (WidgetComponent)
		{
			FloatingWidget = Cast<UFloatingInfo>(WidgetComponent->GetUserWidgetObject());
		}
	}

	if (FloatingWidget != nullptr)
	{
		FloatingWidget->SetMaxHealth(MaxHealth);
	}
}

void UHealthComponent::Initialise(float AMaxHealth)
{
	MaxHealth = AMaxHealth;
	Health = AMaxHealth;
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetOwner()->FindComponentByClass(UWidgetComponent::StaticClass()));
	if (WidgetComponent)
	{
		FloatingWidget = Cast<UFloatingInfo>(WidgetComponent->GetUserWidgetObject());
	}
	if (FloatingWidget)
	{
		FloatingWidget->SetMaxHealth(AMaxHealth);
		FloatingWidget->SetHealth(Health);

	}
}

void UHealthComponent::TakeDamage(float ADamage)
{
	if (Health - ADamage <= 0)
	{
		Health = 0;
		GetOwner()->Destroy();
	}
	else
	{
		Health -= ADamage;
		if (FloatingWidget)
		{
			FloatingWidget->SetHealth(Health);
		}
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();


	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetOwner()->FindComponentByClass(UWidgetComponent::StaticClass()));
	if (WidgetComponent)
	{
		FloatingWidget = Cast<UFloatingInfo>(WidgetComponent->GetUserWidgetObject());
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, Health);
}