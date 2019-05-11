// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemUtils.h"
#include "StructLibrary.h"
#include "FriendRow.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENSE_API UFriendRow : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* FriendButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ButtonText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FriendName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StatusText;

	UPROPERTY(meta = (BindWidget))
	class UImage* FriendAvatar;

	bool Playing = false;

	bool Joinable = false;

	bool Online = false;

	void Setup(uint32 AnIndex, bool AnInviteOrJoin, FOnlineUserPresence APresence);

	void SetAvatar(class UTexture2D* AnAvatar);

	uint32 Index;

	bool InviteOrJoin = true;
	UFUNCTION()
	void OnButtonClicked();
};
