// Copyright Epic Games, Inc. All Rights Reserved.

#include "EatUpAllGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "TimerManager.h"
#include "EatUpAllGameInstance.h"

void AEatUpAllGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if (GameState)
	{
		if (NumberOfPlayers >= 2)
		{
			int32 PlayerArrayNumberOfPlayers = GameState.Get()->PlayerArray.Num();
			UE_LOG(LogTemp, Log, TEXT("NumberOfPlayers is %d"), NumberOfPlayers);
			UE_LOG(LogTemp, Log, TEXT("PlayerArrayNumberOfPlayers is %d"), PlayerArrayNumberOfPlayers);

			// GetWorldTimerManager().SetTimer(GameStartTimer, this, &AEatUpAllGameMode::StartGame, 10);
		}
	}
}

void AEatUpAllGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void AEatUpAllGameMode::StartGame()
{
	auto GameInstance = Cast<UEatUpAllGameInstance>(GetGameInstance());

	if (GameInstance == nullptr) return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	if (World)
	{
		bUseSeamlessTravel = true;

		bool bSuccess = World->ServerTravel(FString("/Game/Maps/MainLevel?listen"));

		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("ServerTravel to MainLevel successful"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ServerTravel to MainLevel failed"));
		}
	}
}