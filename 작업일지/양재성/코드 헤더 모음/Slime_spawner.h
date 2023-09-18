// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FSlimeBubble.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Slime_spawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLIME_TEST_API USlime_spawner : public UActorComponent
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> SlimeBubble;
	UPROPERTY(EditAnywhere)
		int32 PoolSize;

public:	
	// Sets default values for this component's properties
	USlime_spawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
