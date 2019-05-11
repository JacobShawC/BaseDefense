// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Engine/World.h"
#include "BDGameInstance.h"
#include "FriendRow.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "OnlineSubsystemUtils.h"
#include "Public/TimerManager.h"
#include "BDSaveGame.h"
#include "BDGameInstance.h"
#include "SaveRow.h"
void UMainMenu::SetUp()
{
	Instance = Cast<UBDGameInstance>(GetWorld()->GetGameInstance());


	FriendRowWidgetClass = (Instance->Widgets).Find("FriendRow");
	SaveRowWidgetClass = (Instance->Widgets).Find("SaveRow");

	RefreshFriends();
	Instance->OnFriendsUpdated.AddUObject(this, &UMainMenu::RefreshFriends);

	Instance->OnSavesUpdated.AddUObject(this, &UMainMenu::RefreshSaves);
	Instance->RefreshSaves();


	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(Instance, &UBDGameInstance::RefreshFriendsList);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 4.0f, true, 2.0f);
	




}

void UMainMenu::RefreshFriends()
{
	TArray <UFriendRow*> OnlineFriends;
	TArray <UFriendRow*> AwayFriends;
	TArray <UFriendRow*> PlayingOtherGame;
	TArray <UFriendRow*> PlayingThisGame;
	TArray <UFriendRow*> PlayingAndJoinable;
	TArray<UFriendRow*> Widgets;
	UWorld* World = GetWorld();

	FriendsList->ClearChildren();


	//for (TSharedRef<FOnlineFriend> AFriend : Instance->Friends)

	for (int i = 0; i < Instance->Friends.Num(); i++)
	{
		TSharedRef<FOnlineFriend> AFriend = Instance->Friends[i];
		FOnlineUserPresence APresence = AFriend->GetPresence();
		UFriendRow* Row = CreateWidget<UFriendRow>(World, FriendRowWidgetClass->Get());

		if (!ensure(Row != nullptr)) return;

		UTexture2D * Avatar = Instance->GetFriendAvatar(AFriend->GetUserId());
		if (Avatar != nullptr)
		{
			Row->SetAvatar(Avatar);
		}
		Row->FriendName->SetText(FText::FromString(AFriend->GetDisplayName()));
		Row->Setup(i, true, APresence);
		if (APresence.bIsPlayingThisGame && APresence.bIsJoinable)
		{
			PlayingAndJoinable.Add(Row);
		}
		else if (APresence.bIsPlayingThisGame && !APresence.bIsJoinable)
		{
			PlayingThisGame.Add(Row);
		}
		else if (APresence.bIsPlaying && !APresence.bIsPlayingThisGame)
		{
			PlayingOtherGame.Add(Row);
		}
		else if (APresence.bIsOnline && !APresence.bIsPlayingThisGame && APresence.Status.State == EOnlinePresenceState::Online)
		{
			OnlineFriends.Add(Row);
		}
		else if (APresence.bIsOnline && !APresence.bIsPlayingThisGame && APresence.Status.State == EOnlinePresenceState::Away)
		{
			AwayFriends.Add(Row);
		}

	}

	for (UFriendRow* Widget : PlayingAndJoinable)
	{
		FriendsList->AddChild(Widget);
	}

	for (UFriendRow* Widget : PlayingThisGame)
	{
		FriendsList->AddChild(Widget);
	}

	for (UFriendRow* Widget : OnlineFriends)
	{
		FriendsList->AddChild(Widget);
	}

	for (UFriendRow* Widget : AwayFriends)
	{
		FriendsList->AddChild(Widget);
	}

	for (UFriendRow* Widget : PlayingOtherGame)
	{
		FriendsList->AddChild(Widget);
	}
}

void UMainMenu::RefreshSaves()
{
	SaveList->ClearChildren();
	UWorld* World = GetWorld();



	//Add the new game button
	USaveRow* NewSave = CreateWidget<USaveRow>(World, SaveRowWidgetClass->Get());
	NewSave->SetUp();
	SaveList->AddChild(NewSave);


	for (UBDSaveGame* ASave : Instance->Saves)
	{
		USaveRow* SaveGameRow = CreateWidget<USaveRow>(World, SaveRowWidgetClass->Get());
		SaveGameRow->SetUp(ASave);

		SaveList->AddChild(SaveGameRow);
	}
}

void UMainMenu::SetLoading()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(1);
}
