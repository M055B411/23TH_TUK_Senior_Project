// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ECS_Manager_test.generated.h"

UCLASS()
class SLIME_TEST_API AECS_Manager_test : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AECS_Manager_test();
	UPROPERTY(EditAnywhere, BlueprintreadOnly)
		bool bEnableTick;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class Database* DB;
private:
	TArray<class SystemBase*> Systems;
};
