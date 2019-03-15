// Fill out your copyright notice in the Description page of Project Settings.

#include "Level1.h"


void ALevel1::RunLevel()
{
	SpawnWave(EEnemy::Seaman, 1, 1, 2, 3);
	SpawnWave(EEnemy::FirstMate, 2, 3, 2, 3);

	SpawnWave(EEnemy::Seaman, 2, 3, 2, 10);

	SpawnWave(EEnemy::FemalePirate, 1, 3, 2, 17);
	SpawnWave(EEnemy::FemalePirate, 2, 3, 2, 17);

}
