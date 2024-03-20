// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

ASlimeGameMode::ASlimeGameMode()
{
}

void ASlimeGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		PlayerStarts.Add(*It);
	}
}

void ASlimeGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (PlayerStarts.Num() == 0)
	{
		ErrorMessage = TEXT("ServerFull");
	}
}

FString ASlimeGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	if (PlayerStarts.Num() == 0) 
	{
		return FString(TEXT("NO PlayerStart"));
	}
	NewPlayerController->StartSpot = PlayerStarts.Pop();
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}
