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
	AEatUpAllGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};