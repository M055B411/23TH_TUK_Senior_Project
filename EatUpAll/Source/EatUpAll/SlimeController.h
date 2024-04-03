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

private:
	
public:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* MappingContext;

	UPROPERTY()
		class UInputAction* MoveAction;
	UPROPERTY()
		class UInputAction* JumpAction;
	UPROPERTY()
		class UInputAction* LookAction;

};
