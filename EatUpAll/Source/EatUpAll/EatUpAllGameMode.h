// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EatUpAllGameMode.generated.h"

UCLASS(minimalapi)
class AEatUpAllGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	uint32 NumberOfPlayers = 0;

private:
	
	FTimerHandle GameStartTimer;
};