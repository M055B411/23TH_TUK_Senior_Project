// Copyright Epic Games, Inc. All Rights Reserved.

#include "EatUpAllGameMode.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
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
	if (NewPlayer)
	{
		FUniqueNetIdRepl UniqueNetIDRepl;
		if (NewPlayer->IsLocalController())
		{
			ULocalPlayer* LocalPlayerRef = NewPlayer->GetLocalPlayer();
			if (LocalPlayerRef)
			{
				UniqueNetIDRepl = LocalPlayerRef->GetPreferredUniqueNetId();
			}
			else
			{
				UNetConnection* RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
				check(IsValid(RemoteNetConnectionRef));
				UniqueNetIDRepl = RemoteNetConnectionRef->PlayerId;
			}
		}
		else 
		{
			UNetConnection* RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
			check(IsValid(RemoteNetConnectionRef));
			UniqueNetIDRepl = RemoteNetConnectionRef->PlayerId;
		}

		TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIDRepl.GetUniqueNetId();
		check(UniqueNetId != nullptr);
		IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(NewPlayer->GetWorld());
		IOnlineSessionPtr SessionRef = SubsystemRef->GetSessionInterface();
		bool bRegistrationSuccess = SessionRef->RegisterPlayer(FName("MainSession"), *UniqueNetId, false);
		if (bRegistrationSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("Registration SuccessFul"));
		}
	}
}
