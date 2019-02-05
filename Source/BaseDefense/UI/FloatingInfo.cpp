// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingInfo.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"

void UFloatingInfo::Initialise(FString AName, float AMaxHealth)
{
	MaxHealth = AMaxHealth;
	SetHealth(AMaxHealth);
}

void UFloatingInfo::SetHealth(float AHealth)
{
	Health = AHealth;
	SetHealthText(AHealth);
	SetHealthProgressBar(AHealth);
	
}

void UFloatingInfo::SetMaxHealth(float AMaxHealth)
{
	MaxHealth = AMaxHealth;
	//FString HealthString = FString::FromInt(FMath::RoundFromZero(Health)) + "/" + FString::FromInt(MaxHealth);
}

void UFloatingInfo::SetHealthText(float AHealth)
{
	Health = AHealth;
	//FString HealthString = FString::FromInt(FMath::RoundFromZero(Health)) + "/" + FString::FromInt(MaxHealth);
	FString HealthString = FString::SanitizeFloat(Health) + "/" + FString::FromInt(MaxHealth);

	HealthText->SetText(FText::FromString(HealthString));
}
void UFloatingInfo::SetName(FString AHealth)
{
	NameText->SetText(FText::FromString(AHealth));
}

void UFloatingInfo::SetHealthProgressBar(float AHealth)
{
	HealthProgressBar->SetPercent(Health / MaxHealth);
}

void UFloatingInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	this->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UFloatingInfo::SetHealthVisibility(bool AVisiblity)
{
	if (AVisiblity)
	{
		HealthSizeBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		HealthSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFloatingInfo::SetHealthTextVisibility(bool AVisiblity)
{
	if (AVisiblity)
	{
		HealthText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		HealthText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFloatingInfo::SetHealthProgressBarVisibility(bool AVisiblity)
{
	if (AVisiblity)
	{
		HealthProgressBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		HealthProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFloatingInfo::SetNameVisibility(bool AVisiblity)
{
	if (AVisiblity)
	{
		NameText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		NameText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
