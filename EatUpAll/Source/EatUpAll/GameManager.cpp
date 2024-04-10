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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GameEnd Call")));
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

void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GameEnd set")));
}

void AGameManager::Tick(float deltatime)
{
	Super::Tick(deltatime);
}

void AGameManager::SetisSlimeAlive(bool isvalid)
{ 
	isSlimeAlive = isvalid;
}

void AGameManager::SetRemainHunters(int32 number) 
{ 
	RemainHunters = number;
}

void AGameManager::SetIsSlimePhaseTwo(bool isvalid) 
{ 
	isSlimePhaseTwo = isvalid;
}

void AGameManager::SetP1Excavate(bool isvalid) 
{
	P1Excavate = isvalid;
}
void AGameManager::SetP2Excavate(bool isvalid) 
{ 
	P2Excavate = isvalid;
}
