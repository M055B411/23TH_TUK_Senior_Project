// Copyright Epic Games, Inc. All Rights Reserved.

#include "EatUpAllGameMode.h"
#include "GameFramework/GameStateBase.h"
// #include "EatUpAllCharacter.h"
// #include "UObject/ConstructorHelpers.h"

AEatUpAllGameMode::AEatUpAllGameMode(): Super()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Assets/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AEatUpAllGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GameState)
	{
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		if (NumberOfPlayers == 2)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				// Seamless travel�� ������� ���� ����
				bUseSeamlessTravel = true;

				// ���� Ʈ���� ȣ��
				bool bSuccess = World->ServerTravel(FString("/Game/Maps/MainLevel"));

				// �α׸� ���� ���� Ʈ���� ���� ���� Ȯ��
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
	}
}
