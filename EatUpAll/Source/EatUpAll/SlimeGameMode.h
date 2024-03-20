// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SlimeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EATUPALL_API ASlimeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASlimeGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

private:
	TArray<class APlayerStart*> PlayerStarts;
};
