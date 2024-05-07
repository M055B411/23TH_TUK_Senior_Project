// Fill out your copyright notice in the Description page of Project Settings.


#include "EOS_GameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"

void UEOS_GameInstance::LoginWithEOS(FString ID, FString Token, FString LoginType)
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = ID;
			AccountDetails.Token = Token;
			AccountDetails.Type = LoginType;
			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this, &UEOS_GameInstance::LoginWithEOS_Return);
			IdentityPointerRef->Login(0, AccountDetails);
		}
	}
}

FString UEOS_GameInstance::GetPlayerUsername()
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			if (IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				return IdentityPointerRef->GetPlayerNickname(0);
			}
		}
	}
	return FString();

}

bool UEOS_GameInstance::IsPlayerLoggedIn()
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			if (IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				return true;
			}
		}
	}
	return false;
}

void UEOS_GameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccesful)
{
	if (bWasSuccesful)
	{
		GetWorld()->ServerTravel(OpenLevelText);
	}
}

void UEOS_GameInstance::OnDestroySessionCompleted(FName SessionName, bool bWasSuccesful)
{

}

void UEOS_GameInstance::OnFindSessionCompleted(bool bWasSucces, TSharedRef<FOnlineSessionSearch> OnlineSessionSearch)
{
	if (bWasSucces)
	{
		IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
		if (SubsystemRef)
		{
			IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
			if (SessionPtrRef)
			{
				int normalen = OnlineSessionSearch->SearchResults.Num();
				UE_LOG(LogTemp, Error, TEXT("Length %f"), float(normalen));

				if (normalen>0)
				{
					SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnJoinSessionCompleted);
					SessionPtrRef->JoinSession(0, FName("MainSession"), SessionSearch->SearchResults[0]);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Server Not Found"));
					// CreateEOSSession(false, false, 10);
					PlayFabServerRequired.Broadcast();
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Server Not Found"));
		// CreateEOSSession(false, false, 10);
		//PlayFabServerRequired.Broadcast();
	}
}

void UEOS_GameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FString JoinAddress;
			IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
			if (SubsystemRef)
			{
				IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
				if (SessionPtrRef)
				{
					SessionPtrRef->GetResolvedConnectString(FName("MainSession"), JoinAddress);
					UE_LOG(LogTemp, Warning, TEXT("Join Address is %s"), *JoinAddress);
					if (JoinAddress.IsEmpty())
					{
						PlayerControllerRef->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
					}
				}
			}
		}
	}
}

void UEOS_GameInstance::CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections)
// void UEOS_GameInstance::CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections, int64 ServerPort, FString RegionInfo)
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = bIsDedicatedServer;
			SessionCreationInfo.bAllowInvites = false;
			SessionCreationInfo.bIsLANMatch = bIsLanServer;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = false;
			SessionCreationInfo.bUsesPresence = false;
			SessionCreationInfo.bAllowJoinViaPresence = false;
			SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = false;
			SessionCreationInfo.bShouldAdvertise = true;
			// SessionCreationInfo.Settings.Add(FName(TEXT("RegionInfo")), FOnlineSessionSetting(RegionInfo, EOnlineDataAdvertisementType::ViaOnlineService));
			// SessionCreationInfo.Settings.Add(FName(TEXT("PortInfo")), FOnlineSessionSetting(ServerPort, EOnlineDataAdvertisementType::ViaOnlineService));
			SessionCreationInfo.Set(SEARCH_KEYWORDS, FString("RandomHi"), EOnlineDataAdvertisementType::ViaOnlineService);
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnCreateSessionCompleted);
			SessionPtrRef->CreateSession(0, FName("MainSession"), SessionCreationInfo);
		
		}
	}
}

void UEOS_GameInstance::FindSessionAndJoin()
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			// SessionSearch->bIsLanQuery = false;
			// SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, false, EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.SearchParams.Empty();
			SessionSearch->MaxSearchResults = 50;
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnFindSessionCompleted, SessionSearch.ToSharedRef());
			SessionPtrRef->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}

void UEOS_GameInstance::JoinSession()
{

}

void UEOS_GameInstance::DestroySession()
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnDestroySessionCompleted);
			SessionPtrRef->DestroySession(FName("MainSession"));
		}
	}
}


void UEOS_GameInstance::LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error)
{
	if (bWasSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Login Fail Reson - %s"), *Error);
	}
}