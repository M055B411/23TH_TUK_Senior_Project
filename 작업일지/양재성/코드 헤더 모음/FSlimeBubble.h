// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSlimeBubble.generated.h"

UCLASS()
class SLIME_TEST_API AFSlimeBubble : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintreadOnly)
		bool _isattached = false;
	UPROPERTY(EditAnywhere, BlueprintreadOnly)
		float bubblehealth = 10;
	AFSlimeBubble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
