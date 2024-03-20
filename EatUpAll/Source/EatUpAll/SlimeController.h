// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SlimeController.generated.h"

/**
 * 
 */
UCLASS()
class EATUPALL_API ASlimeController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

	UPROPERTY()
		class UInputMappingContext* MappingContext;

	UPROPERTY()
		class UInputAction* MoveAction;
	UPROPERTY()
		class UInputAction* JumpAction;
	UPROPERTY()
		class UInputAction* LookAction;

};
