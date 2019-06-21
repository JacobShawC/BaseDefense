// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMap.h"
#include "Components/Image.h"
#include "Engine/World.h"
#include "Building.h"
#include "EnemyChar.h"
#include "BDGameInstance.h"
#include "BDGameState.h"
#include "BDPlayerState.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "PlayerChar.h"
#include "Layout/Geometry.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "LevelGeneration.h"
#include <GenericPlatformMath.h>

void UMiniMap::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());

	float PlayerRelativeSize = 10;
	float BuildingRelativeSize = 1;
	float EnemyRelativeSize = 1;
	/*if (MapWidth == 0)
	{
		MapWidth = State->OrthoWidth;
		MapPosition = State->MapCapturePosition;
	}
	if (MapWidth != 0)
	{
		PlayerRelativeSize = 4000 / MapWidth * PlayerSize;
		BuildingRelativeSize = 4000 / MapWidth * EnemySize;
		EnemyRelativeSize = 4000 / MapWidth * BuildingSize;
	}*/




	for (auto Iterator = Buildings.CreateIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Key.IsValid())
		{
			if (Iterator->Value == nullptr)
			{
				if (MapWidth == 0)
				{
					MapWidth = State->OrthoWidth;
					MapPosition = State->MapCapturePosition;	
				}


				UImage* Image = NewObject<UImage>(UImage::StaticClass());
				Iterator->Value = Image;
				Image->SetColorAndOpacity(FLinearColor::Green);
				BuildingsPanel->AddChild(Image);
				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Image->Slot;
				CanvasPanelSlot->SetSize(FVector2D(BuildingRelativeSize, BuildingRelativeSize));
				CanvasPanelSlot->SetPosition(GetCanvasLocation(Iterator->Key->GetActorLocation()));
			}
		}
		else
		{
			Iterator.RemoveCurrent();
		}
	}

	for (auto Iterator = EnemyCharacters.CreateIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Key.IsValid())
		{
			if (Iterator->Value == nullptr)
			{
				
				UImage* Image = NewObject<UImage>(UImage::StaticClass());
				Iterator->Value = Image;
				Image->SetColorAndOpacity(FLinearColor::Red);
				EnemiesPanel->AddChild(Image);
				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Image->Slot;
				CanvasPanelSlot->SetSize(FVector2D(EnemyRelativeSize, EnemyRelativeSize));
				CanvasPanelSlot->SetPosition(GetCanvasLocation(Iterator->Key->GetActorLocation()));
			}
			else
			{
				if (MapWidth == 0)
				{

					MapWidth = State->OrthoWidth;
					MapPosition = State->MapCapturePosition;

				}

				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Iterator->Value->Slot;

				CanvasPanelSlot->SetPosition(GetCanvasLocation(Iterator->Key->GetActorLocation()));
			}
		}
		else
		{
			Iterator.RemoveCurrent();
		}
	}


	if (Players.Num() != GetWorld()->GetGameState()->PlayerArray.Num())
	{
		for (APlayerState* AState : GetWorld()->GetGameState()->PlayerArray)
		{
			if (!Players.Contains(AState->GetPawn()))
			{
				Players.Add(AState->GetPawn(), nullptr);
			}
		}
	}

	for (auto Iterator = Players.CreateIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Key.IsValid())
		{
			if (Iterator->Value == nullptr)
			{
				if (MapWidth == 0)
				{

					MapPosition = State->MapCapturePosition;

				}
				UImage* Image = NewObject<UImage>(UImage::StaticClass());

				Iterator->Value = Image;
				Image->SetColorAndOpacity(FLinearColor::Yellow);
				PlayersPanel->AddChild(Image);
				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Image->Slot;
				CanvasPanelSlot->SetSize(FVector2D(PlayerRelativeSize, PlayerRelativeSize));
				FVector2D RelativePosition = GetCanvasLocation(Iterator->Key->GetActorLocation());
				float HalfImageSize = Image->GetCachedGeometry().GetLocalSize().X / 2;
				//CanvasPanelSlot->SetPosition(RelativePosition);
				CanvasPanelSlot->SetPosition(FVector2D(RelativePosition.Y - HalfImageSize, RelativePosition.X - HalfImageSize));

			}
			else
			{
				if (MapWidth == 0)
				{

					MapWidth = State->OrthoWidth;
					MapPosition = State->MapCapturePosition;

				}

				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Iterator->Value->Slot;
				FVector2D RelativePosition = GetCanvasLocation(Iterator->Key->GetActorLocation());
				//CanvasPanelSlot->SetPosition(RelativePosition);
				float HalfImageSize = Iterator->Value->GetCachedGeometry().GetLocalSize().X / 2;
				//CanvasPanelSlot->SetPosition(RelativePosition);
				CanvasPanelSlot->SetPosition(FVector2D(RelativePosition.Y - HalfImageSize, RelativePosition.X - HalfImageSize));
			}
		}
		else
		{
			Iterator.RemoveCurrent();
		}
	}


}

FVector2D UMiniMap::GetCanvasLocation(FVector ALocation)
{

	FVector PercentLocation = FVector(-ALocation.X, ALocation.Y, 0);
	//FVector PercentLocation = FVector(ALocation.Y, -ALocation.X, 0);

	PercentLocation = ALocation * (MiniMapTexture->GetCachedGeometry().GetLocalSize().X / 25600);
	//TempLoc = FVector2D(TempLoc.X - MapPosition.X, TempLoc.Y - MapPosition.Y);
	//TempLoc = FVector2D(-TempLoc.X, -TempLoc.Y);

	//TempLoc = FVector2D(TempLoc.X + MapWidth / 2, -TempLoc.Y + MapWidth / 2);

	//// y = x / i * j - k / 2
	//// y + k/2 = x / i * j
	//// x = ((y + k/2 ) / j) * i
	//float Y = (TempLoc.X / MapWidth * WidgetSize - EnemySize / 2);
	//float X = (TempLoc.Y / MapWidth * WidgetSize - EnemySize / 2);
	PercentLocation = FVector(FGenericPlatformMath::Abs(PercentLocation.X), FGenericPlatformMath::Abs(PercentLocation.Y), 0);
	return FVector2D(PercentLocation);
}

FVector UMiniMap::GetWorldLocationFromCanvas(FVector2D ALocation)
{
	FVector ReturnVector;
	return ReturnVector;

}


void UMiniMap::SetUp()
{

}

void UMiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());
	
	if (State->LevelGenerationActor == nullptr)
	{
		State->LevelGenerationActorUpdated.AddUObject(this, &UMiniMap::SetMiniMap);
	}
	else
	{
		SetMiniMap();
	}

	if (State != nullptr)
	{
		Buildings.Empty();
		for (TWeakObjectPtr<ABuilding> ATempBuilding : State->Buildings)
		{
			Buildings.Add(ATempBuilding, nullptr);

		}
	}

	if (State != nullptr)
	{
		EnemyCharacters.Empty();
		for (TWeakObjectPtr<AEnemyChar> ATempEnemy : State->EnemyCharacters)
		{
			EnemyCharacters.Add(ATempEnemy, nullptr);

		}
	}
}

FReply UMiniMap::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	MiniMapClicked(InGeometry, InMouseEvent);

	return FReply::Handled();
}

FReply UMiniMap::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();

}

FReply UMiniMap::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		MiniMapClicked(InGeometry, InMouseEvent);
	}


	return FReply::Handled();
}

void UMiniMap::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UMiniMap::MiniMapClicked(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());


	FVector2D MouseScreenPosition = InMouseEvent.GetScreenSpacePosition();

	//from top left, x = to the left and y is down
	FVector2D MouseWidgetPosition = InGeometry.AbsoluteToLocal(MouseScreenPosition);

	MapWidth = State->OrthoWidth;
	FVector2D MapCenter = State->MapCapturePosition;
	//unreal uses y/x instead of x/y graph
	FVector2D MapTopLeft = FVector2D(MapCenter.X + MapWidth / 2, MapCenter.Y - MapWidth / 2);
	float MouseWorldChangeX = -((MouseWidgetPosition.Y / InGeometry.GetLocalSize().Y) * MapWidth);
	float MouseWorldChangeY = (MouseWidgetPosition.X / InGeometry.GetLocalSize().X) * MapWidth;
	FVector2D MouseWorldPosition = FVector2D(MapTopLeft.X + MouseWorldChangeX, MapTopLeft.Y + MouseWorldChangeY);
	//GetWorldLocationFromCanvas();

	APlayerChar* PlayerChar = Cast<APlayerChar>(GetOwningPlayerPawn());

	if (PlayerChar != nullptr)
	{
		//PlayerChar->SetActorLocation(FVector(MouseWorldPosition.X - 550, MouseWorldPosition.Y, 800));
		PlayerChar->SetActorLocation(FVector((-MouseWidgetPosition.Y / InGeometry.GetLocalSize().X) * 256 * 100, (MouseWidgetPosition.X / InGeometry.GetLocalSize().X ) * 256 * 100, 0));
	}
}

void UMiniMap::Refresh()
{

}

void UMiniMap::SetMiniMap()
{
	ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());

	if (!State->LevelGenerationActor->HasGenerated)
	{
		State->LevelGenerationActor->OnGenerateWorld.AddUObject(this, &UMiniMap::SetMiniMap);
	}
	else
	{
		MiniMapTexture->SetBrushFromTexture(State->LevelGenerationActor->MiniMapTexture);
	}
}


void UMiniMap::AddBuilding(TWeakObjectPtr<class ABuilding> ABuildingInput)
{
	
		Buildings.Add(ABuildingInput, nullptr);
}

void UMiniMap::AddEnemyCharacter(TWeakObjectPtr<class AEnemyChar> AnEnemyChar)
{
	
	EnemyCharacters.Add(AnEnemyChar, nullptr);
}
