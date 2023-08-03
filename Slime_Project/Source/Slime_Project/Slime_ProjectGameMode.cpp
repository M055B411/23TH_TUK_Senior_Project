// Copyright Epic Games, Inc. All Rights Reserved.

#include "Slime_ProjectGameMode.h"
#include "Slime_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASlime_ProjectGameMode::ASlime_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
