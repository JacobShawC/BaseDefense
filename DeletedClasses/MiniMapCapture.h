// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "MiniMapCapture.generated.h"

/**
 * 
 */
class UTextureRenderTarget2D;


UCLASS(Blueprintable)
class BASEDEFENSE_API AMiniMapCapture : public ASceneCapture2D
{
	GENERATED_BODY()

	// Begin Actor interface

	protected:
	AMiniMapCapture(const FObjectInitializer& ObjectInitializer);
	/** after the game is initialized */
	virtual void BeginPlay();

	public:
	/** update world bounds if camera position or FOV changed */
	virtual void Tick(float DeltaSeconds) override;

#if WITH_EDITOR

	protected:
	/** filter out to apply delta Yaw */
	virtual void EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	// End Actor interface


	// Begin Object interface
	/** revert rotations other than Roll */
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	// End Object interface

#endif // WITH_EDITOR


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MiniMap, meta = (ClampMin = "0", ClampMax = "1024"))
		int32 MiniMapWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MiniMap, meta = (ClampMin = "0", ClampMax = "1024"))
		int32 MiniMapHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MiniMap)
		int32 GroundLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
		float AudioListenerGroundLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
		bool bUseAudioListenerOrientation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
		FVector AudioListenerFrontDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
		FVector AudioListenerRightDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
		FVector AudioListenerLocationOffset;

	protected:

	/** updated world bounds */
	void UpdateWorldBounds();

	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D* MiniMapView;

	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D* RenderTarget;

	class UMaterial* Material;

	/** last known location */
	FVector CachedLocation;

	/** last known FOV*/
	float CachedFOV;

	/** texture was re-sized to fit desired mini map size */
	bool bTextureChanged;

};
