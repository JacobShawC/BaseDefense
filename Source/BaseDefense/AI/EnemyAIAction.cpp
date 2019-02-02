// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIAction.h"
#include "EnemyChar.h"
#include "EnemyAIController.h"
void UEnemyAIAction::Initialise(AEnemyAIController* AnOwner)
{
	AIController = AnOwner;
	if (AIController)
	{
		Char = AIController->MyChar;
		if (Char)
		{
			EnemyData = &Char->EnemyData;
		}
	}
}