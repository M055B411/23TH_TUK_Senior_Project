// Copyright Epic Games, Inc. All Rights Reserved.

#include "EatUpAll_projectGameMode.h"
#include "EatUpAll_projectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEatUpAll_projectGameMode::AEatUpAll_projectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
