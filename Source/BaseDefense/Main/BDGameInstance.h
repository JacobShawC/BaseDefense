// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Main/StructLibrary.h"


#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "OnlineFriendsInterface.h"



#include "UObject/CoreOnline.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystem.h"

#include "BDGameInstance.generated.h"
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FFriendsUpdated);
DECLARE_MULTICAST_DELEGATE(FSavesUpdated);



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

	bool GetMenuController();

	void RefreshServerList();

	void RefreshFriendsList();

	UTexture2D* GetFriendAvatar(TSharedRef < const FUniqueNetId > ANetID);

	void RefreshSaves();
	void CreateSave();
	void LoadSave(class UBDSaveGame* ASave);

private:
	

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnReadFriendsComplete(int32 LocalPlayer, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

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
