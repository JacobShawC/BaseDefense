// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "ThirdParty/Steamworks/Steamv139/sdk/public/steam/steam_api.h"
#include "ThirdParty/Steamworks/Steamv139/sdk/public/steam/isteamuser.h"
#include "ThirdParty/Steamworks/Steamv139/sdk/public/steam/isteamfriends.h"
//#include "Steam/isteamuser.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "GameFramework/SaveGame.h"
#include "UI/MainMenu.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSharingInterface.h"
#include "MenuController.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "BDSaveGame.h"


const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UBDGameInstance::UBDGameInstance(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> GUIBP(TEXT("WidgetBlueprint'/Game/UI/GUIWidget.GUIWidget_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> SlotBP(TEXT("WidgetBlueprint'/Game/UI/SlotWidget.SlotWidget_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> HotbarSlotBP(TEXT("WidgetBlueprint'/Game/UI/HotbarSlotWidget.HotbarSlotWidget_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> CommandBP(TEXT("WidgetBlueprint'/Game/UI/CommandWidget.CommandWidget_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBP(TEXT("WidgetBlueprint'/Game/UI/MainMenuWidget.MainMenuWidget_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> FriendRowBP(TEXT("WidgetBlueprint'/Game/UI/FriendRowWidget.FriendRowWidget_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> SaveRowBP(TEXT("WidgetBlueprint'/Game/UI/SaveRowWidget.SaveRowWidget_C'"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelMesh(TEXT("/Game/PolygonPirates/Meshes/Props/SM_Prop_Barrel_04.SM_Prop_Barrel_04"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonMesh(TEXT("StaticMesh'/Game/PolygonPirates/Meshes/Props/SM_Prop_Cannon_03.SM_Prop_Cannon_03'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CoveredCrate(TEXT("StaticMesh'/Game/PolygonPirates/Meshes/Props/SM_Prop_Crate_Covered_01.SM_Prop_Crate_Covered_01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonBall(TEXT("StaticMesh'/Game/PolygonPirates/Meshes/Props/SM_Prop_CannonBalls_01.SM_Prop_CannonBalls_01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Flowers(TEXT("StaticMesh'/Game/PolygonPirates/Meshes/Environments/SM_Env_Flowers_02.SM_Env_Flowers_02'"));
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> BarrelImage(TEXT("Texture2D'/Game/Textures/Icons/Completed/BarrelIcon.BarrelIcon'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> CannonImage(TEXT("Texture2D'/Game/Textures/Icons/Completed/CannonIcon.CannonIcon'"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SeamanMesh(TEXT("SkeletalMesh'/Game/PolygonPirates/Meshes/Characters/People/SK_Character_Pirate_Seaman_01_Bare.SK_Character_Pirate_Seaman_01_Bare'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FirstMateMesh(TEXT("SkeletalMesh'/Game/PolygonPirates/Meshes/Characters/People/SK_Character_Pirate_First_Mate_01_Bare.SK_Character_Pirate_First_Mate_01_Bare'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FemalePirateMesh(TEXT("SkeletalMesh'/Game/PolygonPirates/Meshes/Characters/People/SK_Character_Female_Pirate_01_Bare.SK_Character_Female_Pirate_01_Bare'"));
	
	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> PirateSeamanAnimBP(TEXT("AnimBlueprint'/Game/PolygonPirates/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));



	Widgets.Add("GUI", GUIBP.Class);
	Widgets.Add("Slot", SlotBP.Class);
	Widgets.Add("HotbarSlot", HotbarSlotBP.Class);
	Widgets.Add("Command", CommandBP.Class);
	Widgets.Add("MainMenu", MainMenuBP.Class);
	Widgets.Add("FriendRow", FriendRowBP.Class);
	Widgets.Add("SaveRow", SaveRowBP.Class);

	Images.Add(EBuilding::Wall, BarrelImage.Object);
	//----------------------------------------------------------------------------------------------------------
	//Buildings
	//----------------------------------------------------------------------------------------------------------

	DefaultPlayerData.MaxHealth = 200;

	//BaseBuilding
	FBuildingData BaseBuildingData;
	BaseBuildingData.Team = ETeam::Ally;
	BaseBuildingData.Mesh = BarrelMesh.Object;
	BaseBuildingData.Thumbnail = BarrelImage.Object;



	//Construction
	FBuildingData ConstructionData = BaseBuildingData;
	ConstructionData.Name = "Construction";
	ConstructionData.Mesh = CoveredCrate.Object;
	ConstructionData.Thumbnail = BarrelImage.Object;
	ConstructionData.Building = EBuilding::Construction;
	ConstructionData.MaxHealth = 1;
	ConstructionData.Cost = 0;
	ConstructionData.CanBeBuffed = false;
	ConstructionData.ConstructionTime = 0.0f;

	
	Buildings.Add(EBuilding::Construction, ConstructionData);


	//Wall
	FBuildingData WallBuildingData = BaseBuildingData;
	WallBuildingData.Name = "Wall";
	WallBuildingData.Mesh = BarrelMesh.Object;
	WallBuildingData.Thumbnail = BarrelImage.Object;
	WallBuildingData.Building = EBuilding::Wall;
	WallBuildingData.MaxHealth = 100;
	WallBuildingData.Cost = 20;
	WallBuildingData.ConstructionTime = 1.0f;

	WallBuildingData.Properties.Add(EBuildingProperty::Regen);
	WallBuildingData.Regeneration.RegenAmount = 1;


	//Wall Levels
	FBuildingUpgrade WallLevel2;
	FBuildingUpgrade WallLevel3;

	//Level 2
	WallLevel2.Upgrades.Add(FBuildingBuffStruct(EBuildingBuffType::Health, EBuffOperator::Multiply, 1.5));
	WallLevel2.UpgradeTime = 3.0f;
	WallLevel2.Cost = 50.0f;
	WallBuildingData.Upgrades.Add(EBuildingUpgrade::Level2, WallLevel2);

	//Level 3
	WallLevel3.Upgrades.Add(FBuildingBuffStruct(EBuildingBuffType::Health, EBuffOperator::Multiply, 2));
	WallLevel3.UpgradeTime = 6.0f;
	WallLevel3.Cost = 100.0f;
	WallBuildingData.Upgrades.Add(EBuildingUpgrade::Level3, WallLevel3);


	Buildings.Add(EBuilding::Wall, WallBuildingData);


	//Farm
	FBuildingData FarmBuildingData = BaseBuildingData;
	FarmBuildingData.Name = "Farm";
	FarmBuildingData.Mesh = Flowers.Object;
	FarmBuildingData.Building = EBuilding::Farm;
	FarmBuildingData.MaxHealth = 50;
	FarmBuildingData.Cost = 100;
	FarmBuildingData.ConstructionTime = 1.f;

	FarmBuildingData.Properties.Add(EBuildingProperty::Regen);
	FarmBuildingData.Regeneration.RegenAmount = 1;

	FarmBuildingData.Properties.Add(EBuildingProperty::Income);
	FarmBuildingData.Income.IncomeAmount = 5;
	FarmBuildingData.Income.Cooldown = 2;

	Buildings.Add(EBuilding::Farm, FarmBuildingData);


	//Arrow Tower
	FBuildingData ArrowTowerBuildingData = BaseBuildingData;

	ArrowTowerBuildingData.Name = "Cannon";
	ArrowTowerBuildingData.Mesh = CannonMesh.Object;
	ArrowTowerBuildingData.Thumbnail = CannonImage.Object;
	ArrowTowerBuildingData.MeshScale = 0.4f;
	ArrowTowerBuildingData.Building = EBuilding::ArrowTower;
	ArrowTowerBuildingData.MaxHealth = 25;
	ArrowTowerBuildingData.Cost = 50;
	ArrowTowerBuildingData.ConstructionTime = 0.5f;

	ArrowTowerBuildingData.Properties.Add(EBuildingProperty::Regen);
	ArrowTowerBuildingData.Regeneration.RegenAmount = 1;
	ArrowTowerBuildingData.Regeneration.Cooldown = 1;

	ArrowTowerBuildingData.Properties.Add(EBuildingProperty::Attack);
	ArrowTowerBuildingData.Attack.AnimationTime = 1;
	ArrowTowerBuildingData.Attack.AttackRule = EAttackRule::Closest;
	ArrowTowerBuildingData.Attack.AttackType = EAttackType::Ranged;
	ArrowTowerBuildingData.Attack.Damage = 5;
	ArrowTowerBuildingData.Attack.ReloadTime = 1;
	ArrowTowerBuildingData.Attack.Range = 200;
	ArrowTowerBuildingData.Attack.Projectile.Mesh = CannonBall.Object;
	ArrowTowerBuildingData.Attack.Projectile.Speed = 300;

	Buildings.Add(EBuilding::ArrowTower, ArrowTowerBuildingData);

	//----------------------------------------------------------------------------------------------------------
	//Enemies
	//----------------------------------------------------------------------------------------------------------
	//Seaman
	FEnemyData SeamanData;
	SeamanData.Name = "Small Zombie";
	SeamanData.Enemy = EEnemy::Seaman;
	SeamanData.MaxHealth = 50;
	SeamanData.MovementSpeed = 5;
	SeamanData.Attack.Damage = 10;
	SeamanData.Attack.ReloadTime = 2;
	SeamanData.Attack.Range = 50;
	SeamanData.Bounty = 50;
	SeamanData.Mesh = SeamanMesh.Object;
	SeamanData.Anim = PirateSeamanAnimBP.Object;

	SeamanData.Attack.AttackRule = EAttackRule::Closest;
	Enemies.Add(EEnemy::Seaman, SeamanData);

	//First Mate
	FEnemyData FirstMateData;
	FirstMateData.Name = "Big Zombie";
	FirstMateData.Enemy = EEnemy::FirstMate;
	FirstMateData.MaxHealth = 200;
	FirstMateData.MovementSpeed = 5;
	FirstMateData.Attack.Damage = 20;
	FirstMateData.Attack.ReloadTime = 2;
	FirstMateData.Attack.Range = 50;
	FirstMateData.Bounty = 100;
	FirstMateData.Mesh = FirstMateMesh.Object;
	FirstMateData.Anim = PirateSeamanAnimBP.Object;

	FirstMateData.Attack.AttackRule = EAttackRule::Closest;
	Enemies.Add(EEnemy::FirstMate, FirstMateData);

	//Female Pirate
	FEnemyData FemalePirateData;
	FemalePirateData.Name = "Female Pirate";
	FemalePirateData.Enemy = EEnemy::FemalePirate;
	FemalePirateData.MaxHealth = 500;
	FemalePirateData.MovementSpeed = 5;
	FemalePirateData.Attack.Damage = 50;
	FemalePirateData.Attack.ReloadTime = 2;
	FemalePirateData.Attack.Range = 50;
	FemalePirateData.Bounty = 150;
	FemalePirateData.Mesh = FemalePirateMesh.Object;
	FemalePirateData.Anim = PirateSeamanAnimBP.Object;

	FemalePirateData.Attack.AttackRule = EAttackRule::Closest;
	Enemies.Add(EEnemy::FemalePirate, FemalePirateData);



}


void UBDGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		FriendsInterface = Subsystem->GetFriendsInterface();

		if (FriendsInterface.IsValid())
		{
			RefreshFriendsList();
		}

		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBDGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBDGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBDGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBDGameInstance::OnJoinSessionComplete);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}
}


void UBDGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UBDGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success) {
		CreateSession();
	}
}



void UBDGameInstance::CreateSession()
{
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UBDGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld * World = GetWorld();
	if (!ensure(World != nullptr)) return;

	//World->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");

	/*bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/TopDownExampleMap?listen");*/
	//StartSession();

}

void UBDGameInstance::RefreshFriendsList()
{

	if (FriendsInterface.IsValid())
	{
		FOnReadFriendsListComplete Delegate;
		Delegate.BindUObject(this, &UBDGameInstance::OnReadFriendsComplete);

		IOnlineSubsystem::Get()->GetFriendsInterface()->ReadFriendsList(0, TEXT("TestFriends"), Delegate);
	}
}


UTexture2D* UBDGameInstance::GetFriendAvatar(TSharedRef < const FUniqueNetId > ANetID)
{
	//uint64 i64 = 0;

	//FString NetString = ANetID->ToString();
	//int convCount = sscanf_s(TCHAR_TO_ANSI(*NetString), "%llu", &i64);
	uint64 i64 = FCString::Strtoui64(*ANetID->ToString(), NULL, 10);
	uint32 Width;
	uint32 Height;

	if (SteamAPI_Init())
	{
		int test = SteamFriends()->GetClanCount();
		//Getting the PictureID from the SteamAPI and getting the Size with the ID
		int Picture = SteamFriends()->GetMediumFriendAvatar(CSteamID(i64));
		SteamUtils()->GetImageSize(Picture, &Width, &Height);


		if (Width > 0 && Height > 0)
		{
			//Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
			BYTE* oAvatarRGBA = new BYTE[Width * Height * 4];

			//Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTextur2D to parse the RGBA Steam in
			SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));
			UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);

			//MAGIC!
			uint8* MipData = (uint8*)Avatar->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
			Avatar->PlatformData->Mips[0].BulkData.Unlock();

#ifdef WITH_EDITORONLY_DATA

#endif //WITH_EDITORONLY_DATA


			//Setting some Parameters for the Texture and finally returning it
			//Avatar->PlatformData->NumSlices = 1;
			//Avatar->NeverStream = true;
			//Avatar->Rename(TEXT("Avatar"));
			//Avatar->CompressionSettings = TC_EditorIcon;

			Avatar->UpdateResource();

			return Avatar;
		}

		return nullptr;
	}

	return nullptr;
}

void UBDGameInstance::RefreshSaves()
{
	TArray<FString> SavesNames;
	TArray<UBDSaveGame*> TempSaves;
	FString SaveDirectory = "C:/Users/Ziggy/Documents/Unreal Projects/BaseDefense/Saved/Cooked/WindowsNoEditor/BaseDefense/Saved/SaveGames";
	
	FString Test = FPlatformProcess::UserDir();
	
	//SaveDirectory = FPlatformProcess::UserSettingsDir();
//
//#if WITH_EDITOR
//#else
//#endif

	FString FileExtension = ".sav";

	IFileManager::Get().FindFiles(SavesNames, *SaveDirectory, *FileExtension);
	for (int i = 0; i < SavesNames.Num(); i++)
	{
		SavesNames[i].RemoveFromEnd(".sav", ESearchCase::IgnoreCase);
	}

	for (FString ASaveName : SavesNames)
	{
		UBDSaveGame* LoadGameInstance = Cast<UBDSaveGame>(UGameplayStatics::CreateSaveGameObject(UBDSaveGame::StaticClass()));
		//USaveGame* TempSave = UGameplayStatics::LoadGameFromSlot(ASaveName, LoadGameInstance->UserIndex);
		USaveGame* TempSave = UGameplayStatics::LoadGameFromSlot(ASaveName, LoadGameInstance->UserIndex);
		LoadGameInstance = Cast<UBDSaveGame>(TempSave);
		if (LoadGameInstance != nullptr)
		{
			TempSaves.Add(LoadGameInstance);
		}
	}


	Saves = TempSaves;

	OnSavesUpdated.Broadcast();
}

void UBDGameInstance::CreateSave()
{
	UBDSaveGame* SaveGameInstance = Cast<UBDSaveGame>(UGameplayStatics::CreateSaveGameObject(UBDSaveGame::StaticClass()));
	FString SaveName = "BDGameSave" + FString::FromInt(Saves.Num() + 1);
	SaveGameInstance->SaveSlotName = SaveName;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
	RefreshSaves();
}

void UBDGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UBDGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));

		TArray<FServerData> ServerNames;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}
			ServerNames.Add(Data);
		}
	}
}

void UBDGameInstance::JoinFriend(int Index)
{
	TArray< TSharedRef<FOnlineFriend> > FriendsTemp;


	if (Friends.Num() > ((int)(Index)+1))
	{
		FOnlineFriend* AFriend = &(Friends[Index].Get());
		SessionInterface->FindFriendSession(0, *(AFriend->GetUserId()));
	}
}

void UBDGameInstance::InviteFriend(int Index)
{
	if (Friends.Num() > ((int)(Index)+1))
	{
		FOnlineFriend* AFriend = &(Friends[Index].Get());

		SessionInterface->SendSessionInviteToFriend(0, SESSION_NAME, *(AFriend->GetUserId()));
	}
}

void UBDGameInstance::OnSessionUserInviteAccepted(bool bWasSuccessful, int32 ControllerId, TSharedPtr<const FUniqueNetId> InUserId, const FOnlineSessionSearchResult & SearchResult)
{
	UE_LOG_ONLINE_SESSION(Verbose, TEXT("OnSessionInviteAccepted ControllerId: %d bSuccess: %d"), ControllerId, bWasSuccessful);
	// Don't clear invite accept delegate

	if (bWasSuccessful)
	{
		SessionInterface->JoinSession(ControllerId, SESSION_NAME, SearchResult);
	}
}

void UBDGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;


	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UBDGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController * PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}


void UBDGameInstance::OnReadFriendsComplete(int32 LocalPlayer, bool bWasSuccessful, const FString & ListName, const FString & ErrorStr)
{
	TArray< TSharedRef<FOnlineFriend> > TempFriends;
	if (IOnlineSubsystem::Get()->GetFriendsInterface()->GetFriendsList(LocalPlayer, ListName, TempFriends))
	{
		Friends = TempFriends;
		OnFriendsUpdated.Broadcast();

	}



}

void UBDGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UBDGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

bool UBDGameInstance::GetMenuController()
{
	MenuController = Cast<AMenuController>((GetFirstLocalPlayerController()));
	if (!ensure(MenuController != nullptr)) return false;
	else return true;
}
