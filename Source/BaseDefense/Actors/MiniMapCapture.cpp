// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/Material.h"
#include "Classes/Engine/World.h"
#include "BDGameState.h"

AMiniMapCapture::AMiniMapCapture(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Game/Textures/MiniMap/InvertMiniMap.InvertMiniMap'"));
	Material = MaterialAsset.Object;
	GetCaptureComponent2D()->bCaptureEveryFrame = false;
	GetCaptureComponent2D()->ProjectionType = ECameraProjectionMode::Orthographic;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	MiniMapWidth = 512;
	MiniMapHeight = 512;
	RootComponent->SetWorldRotation(FRotator(-90, 0, 0));
	AudioListenerGroundLevel = 500.0f;
	bUseAudioListenerOrientation = false;
	bTextureChanged = true;
}

void AMiniMapCapture::BeginPlay()
{
	// @todo clean up
	Super::BeginPlay();

	if (!GetCaptureComponent2D()->TextureTarget || MiniMapWidth != GetCaptureComponent2D()->TextureTarget->GetSurfaceWidth()
		|| MiniMapHeight != GetCaptureComponent2D()->TextureTarget->GetSurfaceHeight())
	{
		MiniMapView = NewObject<UTextureRenderTarget2D>();
		MiniMapView->InitAutoFormat(MiniMapWidth, MiniMapHeight);
		GetCaptureComponent2D()->TextureTarget = MiniMapView;
		bTextureChanged = true;
	}

	// Ensure that rotation is correct.
	RootComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
	ABDGameState* const MyGameState = GetWorld()->GetGameState<ABDGameState>();
	if (MyGameState != nullptr)
	{
		MyGameState->MiniMapCamera = this;
		MyGameState->SetMapProperties(this->GetActorLocation(), GetCaptureComponent2D()->OrthoWidth);
	}
	CachedFOV = GetCaptureComponent2D()->FOVAngle;
	CachedLocation = RootComponent->GetComponentLocation();
	UpdateWorldBounds();
}

void AMiniMapCapture::UpdateWorldBounds()
{
	ABDGameState* const MyGameState = GetWorld()->GetGameState<ABDGameState>();
	if (MyGameState != nullptr)
	{
		TArray<FVector> Points;
		FVector const CamLocation = RootComponent->GetComponentLocation();
		float DistanceFromGround = CamLocation.Z - GroundLevel;
		float Alpha = FMath::DegreesToRadians(GetCaptureComponent2D()->FOVAngle / 2);
		float MaxVisibleDistance = (DistanceFromGround / FMath::Cos(Alpha)) * FMath::Sin(Alpha);

		Points.Add(FVector(CamLocation.X + MaxVisibleDistance, CamLocation.Y + MaxVisibleDistance, GroundLevel));
		Points.Add(FVector(CamLocation.X - MaxVisibleDistance, CamLocation.Y - MaxVisibleDistance, GroundLevel));

		MyGameState->WorldBounds = FBox(Points);

		GetCaptureComponent2D()->UpdateContent();


		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
		//MaterialInstance->SetTextureParameterValue(FName(TEXT("Texture")), MiniMapView);
		MaterialInstance->SetScalarParameterValue(FName(TEXT("Opacity")), 0);
		
		//UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, FLinearColor(1, 0, 0, 0));
		UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, FLinearColor(0, 0, 0, 1));

		
		UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), RenderTarget, MaterialInstance);
	}
}

void AMiniMapCapture::Tick(float DeltaSeconds)
{
	if (CachedFOV != GetCaptureComponent2D()->FOVAngle || CachedLocation != RootComponent->GetComponentLocation() || bTextureChanged)
	{
		bTextureChanged = false;
		CachedFOV = GetCaptureComponent2D()->FOVAngle;
		CachedLocation = RootComponent->GetComponentLocation();
		UpdateWorldBounds();
	}
}

#if WITH_EDITOR
void AMiniMapCapture::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != nullptr ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == FName(TEXT("RelativeRotation")))
	{
		FRotator ChangedRotation = RootComponent->GetComponentRotation();
		RootComponent->SetWorldRotation(FRotator(-90, 0, ChangedRotation.Roll));
	}
}

void AMiniMapCapture::EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	FRotator FiltredRotation(0, DeltaRotation.Yaw, 0);
	Super::EditorApplyRotation(FiltredRotation, bAltDown, bShiftDown, bCtrlDown);
}

#endif