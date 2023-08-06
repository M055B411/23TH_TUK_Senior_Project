// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Slime_ProjectGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLIME_PROJECT_API USlime_ProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USlime_ProjectGameInstance(const FObjectInitializer & objectInitalizer);

	virtual void Init();

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& Address);
};
