// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Main/StructLibrary.h"

#include "CoreMinimal.h"
#include "Misc/CoreMisc.h"
#include "Containers/Ticker.h"
#include "UObject/CoreOnline.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "OnlineSessionInterface.h"
#include "OnlineFriendsInterface.h"

#include "OnlineSessionSettings.h"

#include "UObject/CoreOnline.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystem.h"

#include "BDGameInstance.generated.h"
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FFriendsUpdated);
DECLARE_MULTICAST_DELEGATE(FSavesUpdated);

class FOnlineSessionSearchResult;

UCLASS()
class BASEDEFENSE_API UBDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UBDGameInstance(const FObjectInitializer& ObjectInitializer);

public:



	virtual void Init() override;

	UFUNCTION(Exec)
	void Host(FString ServerName);

	UFUNCTION(Exec)
	void Join(uint32 Index);

	UFUNCTION()
	void JoinFriend(int Index);

	void OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 LocalUserNum, TSharedPtr< const FUniqueNetId > UserId, const FOnlineSessionSearchResult& SearchResult);
	
	UFUNCTION()
	void InviteFriend(int Index);

	void StartSession();

	virtual void LoadMainMenu();

	void LoadLevel(ELevel ALevel);
	bool GetMenuController();

	void RefreshServerList();

	void RefreshFriendsList();

	UTexture2D* GetFriendAvatar(TSharedRef < const FUniqueNetId > ANetID);

	void RefreshSaves();
	void CreateSave();
	void LoadSave(class UBDSaveGame* ASave);
	void RefreshLevelRewards();

private:
	
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool Success);
	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool Success);
	UFUNCTION()
	void OnFindSessionsComplete(bool Success);
	/*UFUNCTION()
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);*/
	UFUNCTION()
	void OnReadFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);
	void OnFindFriendSessionComplete(int32 LocalUserNum, bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SearchResult);
	void OnJoinSessionComplete(FName AServerName, EOnJoinSessionCompleteResult::Type AResult);
	

	FString DesiredServerName;
	void CreateSession();

private:
	UPROPERTY()
	class AMenuController* MenuController = nullptr;
	IOnlineSessionPtr SessionInterface;
	IOnlineFriendsPtr FriendsInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

public:
	TArray< TSharedRef<FOnlineFriend> > Friends;
	FFriendsUpdated OnFriendsUpdated;
	FSavesUpdated OnSavesUpdated;


	TMap<FString, TSubclassOf<class UUserWidget>> Widgets;
	TMap<EBuilding, class UTexture2D*> Images;
	TMap<EBuilding, FBuildingData> Buildings;
	TMap<EEnemy, FEnemyData> Enemies;
	TMap<ELevel, FLevelData> Levels;
	FPlayerData DefaultPlayerData;
	UPROPERTY()
	TArray<class UBDSaveGame*> Saves;

	UPROPERTY()
	class UBDSaveGame* CurrentSave = nullptr;
};
