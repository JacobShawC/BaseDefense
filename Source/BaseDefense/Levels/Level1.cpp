// Fill out your copyright notice in the Description page of Project Settings.

#include "Level1.h"


void ALevel1::RunLevel()
{

	//Wave 1
	SpawnWave(EEnemy::ElementalGolem, 1, 5, 0, 59);

	//2
	SpawnWave(EEnemy::ElementalGolem, 1, 10, 60, 119);
	SpawnWave(EEnemy::FortGolem, 1, 10, 65, 119);
	//3
	SpawnWave(EEnemy::FortGolem, 1, 5, 120, 179);
	//4
	SpawnWave(EEnemy::FortGolem, 1, 10, 180, 239);
	SpawnWave(EEnemy::MechanicalGolem, 1, 10, 185, 239);
	//5
	SpawnWave(EEnemy::MechanicalGolem, 1, 5, 240, 300);

}
