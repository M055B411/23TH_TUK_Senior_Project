// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Hunter_Controller.h"
#include "EOS_GameInstance.h"
#include "EatUpAllGameMode.h"

void AHunter_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}

void AHunter_Controller::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =   ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

//void AHunter_Controller::OnNetCleanup(UNetConnection* Connection)
//{
//	UEOS_GameInstance* GameInstanceRef = Cast<UEOS_GameInstance>(GetWorld()->GetGameInstance());
//	if (GameInstanceRef)
//	{
//		GameInstanceRef->DestroySession();
//	}
//	Super::OnNetCleanup(Connection);
//}