// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EOS_GameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FServerRequired);
/**
 * 
 */
UCLASS()
class EATUPALL_API UEOS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
	FString GetPlayerUsername();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
	bool IsPlayerLoggedIn();

	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
	void CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections);
	// void CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections, int64 ServerPort, FString RegionInfo);

	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
	void FindSessionAndJoin();

	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
	void JoinSession();

	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
	void DestroySession();

	UPROPERTY(BlueprintAssignable, Category = "EOS Delegates")
	FServerRequired PlayFabServerRequired;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
	FString OpenLevelText;

	void LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error);
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccesful);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccesful);
	void OnFindSessionCompleted(bool bWasSucces, TSharedRef<FOnlineSessionSearch> OnlineSessionSearch);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
