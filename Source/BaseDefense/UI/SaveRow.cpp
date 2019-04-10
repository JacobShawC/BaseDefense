// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BDGameInstance.h"
#include "BDGameInstance.h"

void USaveRow::SetUp(FString ASaveName)
{
	Instance = Cast<UBDGameInstance>(GetWorld()->GetGameInstance());

	NewGame = false;
	ButtonText->SetText(FText::FromString(ASaveName));

	SaveButton->OnClicked.AddDynamic(this, &USaveRow::OnButtonClicked);
	SaveName = ASaveName;
}

void USaveRow::SetUp()
{
	Instance = Cast<UBDGameInstance>(GetWorld()->GetGameInstance());

	FString ButtonTextTemp = "New Game";
	ButtonText->SetText(FText::FromString(ButtonTextTemp));


	NewGame = true;

	SaveButton->OnClicked.AddDynamic(this, &USaveRow::OnButtonClicked);
}

void USaveRow::OnButtonClicked()
{
	if (NewGame)
	{
		if (Instance != nullptr)
		{
			Instance->CreateSave();
		}
	}
	else
	{
		//load save
	}
}
