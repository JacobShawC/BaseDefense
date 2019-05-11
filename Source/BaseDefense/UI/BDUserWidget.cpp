// Fill out your copyright notice in the Description page of Project Settings.

#include "BDUserWidget.h"
#include "Controllers/BDPlayerController.h"
#include "Animation/WidgetAnimation.h"
#include "Engine/World.h"

void UBDUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (GetWorld() != nullptr && GetWorld()->GetFirstPlayerController() != nullptr)
	{
		ABDPlayerController* Controller = nullptr;
		Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController());
		if (Controller != nullptr)
		{
			Controller->CurrentlySelected = this;
		}
	}
}

void UBDUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	/*
	if (Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController())->CurrentlySelected == this)
	{
		Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController())->CurrentlySelected = nullptr;
	}*/

	if (GetWorld() != nullptr && GetWorld()->GetFirstPlayerController() != nullptr)
	{
		ABDPlayerController* Controller = nullptr;
		Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController());
		if (Controller != nullptr)
		{
			if (Controller->CurrentlySelected == this)
			{
				Controller->CurrentlySelected = nullptr;
			}
		}
	}

}

FReply UBDUserWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{

	Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController())->SelectPressed();

	return FReply::Handled();
}

FReply UBDUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Cast<ABDPlayerController>(GetWorld()->GetFirstPlayerController())->SelectReleased();

	return FReply::Handled();
}

void UBDUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GetAnimations();
}

void UBDUserWidget::GetAnimations()
{
	AnimationsMap.Empty();

	UProperty* Prop = GetClass()->PropertyLink;

	// Run through all properties of this class to find any widget animations
	while (Prop != nullptr)
	{
		// Only interested in object properties
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			// Only want the properties that are widget animations
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);

				if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
				{
					FName AnimName = WidgetAnim->MovieScene->GetFName();
					AnimationsMap.Add(AnimName, WidgetAnim);
				}
			}
		}
		Prop = Prop->PropertyLinkNext;
	}
}
