// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "FloatingInfo.h"
#include "Components/WidgetComponent.h"
#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"
#include "DamageTextActor.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Death.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);

	// ...
}



void UHealthComponent::OnRep_SetHealth()
{
	//SpawnDamageText();
	//OnRep_LastDamage();
	CheckFull();


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
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().ClearTimer(FuzeTimerHandle);

		FloatingWidget->SetMaxHealth(MaxHealth);
		FloatingWidget->SetHealth(Health);
	}
	else
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UHealthComponent::OnRep_SetHealth, 0.05f, false);

	}
}

//void UHealthComponent::OnRep_LastDamage()
//{
//	if (LastDamage > 0 && GetOwner()->WasRecentlyRendered())
//	{
//		SpawnDamageText(FString::SanitizeFloat(LastDamage), FColor::Red);
//	}
//}




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
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().ClearTimer(FuzeTimerHandle);

		FloatingWidget->SetMaxHealth(MaxHealth);
	}
	else
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UHealthComponent::OnRep_SetMaxHealth, 0.05f, false);

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

void UHealthComponent::TakeDamage(AActor* AnActor, float ADamage)
{

	if (Health - ADamage <= 0)
	{
		Health = 0;
		AActor* Owner = nullptr;
		Owner = GetOwner();
		if (Owner != nullptr)
		{
			// if health is 0 kill using interface or just destroy object
			if (UKismetSystemLibrary::DoesImplementInterface(Owner, UDeath::StaticClass()))
			{
				IDeath* Interface = Cast<IDeath>(Owner);
				Interface->Kill();
			}
			else
			{
				Owner->Destroy();
			}

		}


	}
	else
	{
		Health -= ADamage;
		if (FloatingWidget)
		{
			FloatingWidget->SetHealth(Health);
		}
	}

	//SpawnDamageText();
	//OnRep_LastDamage();
	MulticastSpawnDamageText(FString::SanitizeFloat(ADamage), FColor::Red);



	OnAttacked.Broadcast(AnActor, ADamage);
}

float UHealthComponent::Heal(float AHeal)
{

	float ReturnFloat = 0;
	if (Health + AHeal > MaxHealth)
	{
		ReturnFloat = Health + AHeal - MaxHealth;
		Health = MaxHealth;
	}
	else
	{
		Health += AHeal;
		ReturnFloat = AHeal;
	}
	if (FloatingWidget)
	{
		FloatingWidget->SetHealth(Health);
	}


	return ReturnFloat;
}

void UHealthComponent::ChangeMaxHealth(float AMaxHealth, bool ChangesCurrentHealth)
{
	if (MaxHealth != AMaxHealth)
	{
		if (ChangesCurrentHealth)
		{
			float Ratio = AMaxHealth / MaxHealth;
			MaxHealth = AMaxHealth;
			Health = Health * Ratio;
		}
		else
		{
			MaxHealth = AMaxHealth;
		}

		OnRep_SetHealth();
	}

}

void UHealthComponent::CheckFull()
{
	if (Health == MaxHealth)
	{
		if (LastKnownFull == true)
		{

		}
		else
		{
			HealthFullDelegate.Broadcast(true);
		}
	}

	if (Health < MaxHealth)
	{
		if (LastKnownFull == false)
		{

		}
		else
		{
			HealthFullDelegate.Broadcast(false);
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


void UHealthComponent::MulticastSpawnDamageText_Implementation(const FString& AText, FColor AColor)
{
	if (GetOwner()->WasRecentlyRendered())
	{
		ADamageTextActor* DamageTextActor = nullptr;
		FVector OwnerLoc = GetOwner()->GetActorLocation();
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		DamageTextActor = Cast<ADamageTextActor>(GetOwner()->GetWorld()->SpawnActor<ADamageTextActor>(ADamageTextActor::StaticClass(), OwnerLoc, FRotator(0.0f), SpawnParams));
		if (DamageTextActor != nullptr)
		{
			DamageTextActor->Initialise(AText, AColor);
		}
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, Health);
}