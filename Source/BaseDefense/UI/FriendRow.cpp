// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendRow.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "OnlineSubsystemUtils.h"

#include "BDGameInstance.h"
void UFriendRow::Setup(uint32 AnIndex, bool AnInviteOrJoin, FOnlineUserPresence APresence)
{
	if (APresence.bIsPlayingThisGame && APresence.bIsJoinable)
	{
		StatusText->SetText(FText::FromString("Playing BaseDefenseGame"));
		FriendButton->OnClicked.AddDynamic(this, &UFriendRow::OnButtonClicked);
		StatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));

	}
	else if (APresence.bIsPlayingThisGame && !APresence.bIsJoinable)
	{
		StatusText->SetText(FText::FromString("Playing BaseDefenseGame"));
		FriendButton->SetVisibility(ESlateVisibility::Collapsed);
		StatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));

	}
	else if (APresence.bIsPlaying)
	{
		StatusText->SetText(FText::FromString("Playing Other Game"));
		FriendButton->SetVisibility(ESlateVisibility::Collapsed);
		StatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));

	}
	else if (APresence.Status.State == EOnlinePresenceState::Away)
	{
		StatusText->SetText(FText::FromString("Away"));
		FriendButton->SetVisibility(ESlateVisibility::Collapsed);
		StatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));

	}
	else if (APresence.bIsOnline)
	{
		StatusText->SetText(FText::FromString("Online"));
		FriendButton->SetVisibility(ESlateVisibility::Collapsed);
		StatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
	}

	InviteOrJoin = AnInviteOrJoin;
	Index = AnIndex;
}

void UFriendRow::SetAvatar(UTexture2D* AnAvatar)
{
	FriendAvatar->SetBrushFromTexture(AnAvatar);
}

void UFriendRow::OnButtonClicked()
{
	UBDGameInstance* Instance = Cast<UBDGameInstance>(GetWorld()->GetGameInstance());
	if (InviteOrJoin && Instance)
	{
		Instance->JoinFriend(Index);
	}
	else
	{
		Instance->InviteFriend(Index);
	}
	
}
