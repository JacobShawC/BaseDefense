// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BDGameInstance.h"
#include "BDSaveGame.h"
#include "BDGameInstance.h"

void USaveRow::SetUp(UBDSaveGame* ASave)
{
	Instance = Cast<UBDGameInstance>(GetWorld()->GetGameInstance());

	if (ASave != nullptr)
	{
		Save = ASave;
		NewGame = false;
		ButtonText->SetText(FText::FromString(ASave->SaveSlotName));

		SaveButton->OnClicked.AddDynamic(this, &USaveRow::OnButtonClicked);
		SaveName = ASave->SaveSlotName;
	}
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
		Instance->LoadSave(Save);
	}
}
