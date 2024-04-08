// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "EngineUtils.h"
#include "VacuumPlayer.h"

void AGameManager::CountRemainHunter()
{
	TArray<AVacuumPlayer*> Hunters;
	for (TActorIterator<AVacuumPlayer> It(GetWorld()); It; ++It)
	{
		Hunters.Add(*It);
	}

	SetRemainHunters(Hunters.Num());
}

void AGameManager::EndGame()
{
}

void AGameManager::SummaryGame()
{
	if (isSlimePhaseTwo) 
	{

	}
	else 
	{

	}
}
