// Fill out your copyright notice in the Description page of Project Settings.

#include "Hunter_Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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