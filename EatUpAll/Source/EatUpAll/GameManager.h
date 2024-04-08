// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameManager.generated.h"

/**
 * 
 */
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

public:
	void SetisSlimeAlive(bool isvalid) { isSlimeAlive = isvalid; }

	void SetRemainHunters(int32 number) { RemainHunters = number; }

	void SetIsSlimePhaseTwo(bool isvalid) { isSlimePhaseTwo = isvalid; }

	void SetP1Excavate(bool isvalid) { P1Excavate = isvalid; }
	void SetP2Excavate(bool isvalid) { P2Excavate = isvalid; }

	void CountRemainHunter();

	void EndGame();

	void SummaryGame();
};
