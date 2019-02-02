// Fill out your copyright notice in the Description page of Project Settings.
#include "BuildingAttackComponent.h"

#include "Components/CapsuleComponent.h"

void UBuildingAttackComponent::Initialise(FBuildingData ABuildingData, AActor* AnOwner)
{
	BuildingData = ABuildingData;

	Owner = AnOwner;
	if (Owner)
	{
		RangeCapsule = NewObject<UCapsuleComponent>(AnOwner);

		RangeCapsule->SetCollisionProfileName("AllyRange");
		RangeCapsule->SetGenerateOverlapEvents(true);
		RangeCapsule->SetCapsuleSize(ABuildingData.Attack.Range, 50, true);
		RangeCapsule->AttachTo(AnOwner->GetRootComponent());
		RangeCapsule->RegisterComponent();

		RangeCapsule->bVisible = true;
		if (RangeCapsule)
		{
			RangeCapsule->OnComponentBeginOverlap.AddDynamic(this, &UBuildingAttackComponent::OnRangeOverlap);
		}
	}
}

void UBuildingAttackComponent::OnRangeOverlap(UPrimitiveComponent* AComponent, AActor* AnActor, UPrimitiveComponent* OtherComponent, int32 ABodyIndex, bool bFromSweep, const FHitResult& ASweepResult)
{

}
