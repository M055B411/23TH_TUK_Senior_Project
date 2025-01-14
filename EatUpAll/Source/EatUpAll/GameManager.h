// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameManager.generated.h"

/**
 * 
 */
//DECLARE_MULTICAST_DELEGATE(FScoreChecker)

UCLASS()
class EATUPALL_API AGameManager : public ALevelScriptActor
{
	GENERATED_BODY()


private:
	bool isSlimeAlive;

	int32 RemainHunters;

	bool isSlimePhaseTwo;

	bool P1Excavate;

	bool P2Excavate;

	TArray<AActor*> Hunters;

	TArray<AActor*> Slime;

public:
	virtual void BeginPlay() override;

	virtual void Tick(float deltatime) override;

	void SetisSlimeAlive(bool isvalid);

	void SetRemainHunters(int32 number);

	void SetIsSlimePhaseTwo(bool isvalid);

	void SetP1Excavate(bool isvalid);
	void SetP2Excavate(bool isvalid);

	void CountRemainHunter();

	void EndGame();

	void SummaryGame();

	float WorldTime;
};
