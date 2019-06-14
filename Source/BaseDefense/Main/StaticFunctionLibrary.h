// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "StaticFunctionLibrary.generated.h"

/**
 * 
 */

UCLASS()
class BASEDEFENSE_API UStaticFunctionLibrary : public UObject
{
	GENERATED_UCLASS_BODY()
private:

	static unsigned char perm[];
	static float  grad(int hash, float x);
	static float  grad(int hash, float x, float y);
	static float  grad(int hash, float x, float y, float z);
	static float  grad(int hash, float x, float y, float z, float t);

public:

	UFUNCTION()
	static void setNoiseSeed(const int32& newSeed);

	UFUNCTION()
	static float SimplexNoise1D(float x);

	UFUNCTION()
	static float SimplexNoise2D(float x, float y);

	UFUNCTION()
	static float SimplexNoise3D(float x, float y, float z);

	UFUNCTION()
	static float SimplexNoise4D(float x, float y, float z, float w);

	// Scaled by float value
	UFUNCTION()
	static float SimplexNoiseScaled1D(float x, float s);

	UFUNCTION()
	static float SimplexNoiseScaled2D(float x, float y, float s);

	UFUNCTION()
	static float SimplexNoiseScaled3D(float x, float y, float z, float s);

	UFUNCTION()
	static float SimplexNoiseScaled4D(float x, float y, float z, float w, float s);

	// Return value in Range between two float numbers
	// Return Value is scaled by difference between rangeMin & rangeMax value

	UFUNCTION()
	static float SimplexNoiseInRange1D(float x, float rangeMin, float rangeMax);

	UFUNCTION()
	static float SimplexNoiseInRange2D(float x, float y, float rangeMin, float rangeMax);

	UFUNCTION()
	static float SimplexNoiseInRange3D(float x, float y, float z, float rangeMin, float rangeMax);

	UFUNCTION()
	static float SimplexNoiseInRange4D(float x, float y, float z, float w, float rangeMin, float rangeMax);

};

