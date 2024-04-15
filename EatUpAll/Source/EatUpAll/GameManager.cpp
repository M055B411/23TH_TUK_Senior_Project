// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "EngineUtils.h"
#include "VacuumPlayer.h"
#include "Slime_Control.h"

void AGameManager::CountRemainHunter()
{
	

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

	WorldTime = 0.f;

	for (TActorIterator<AVacuumPlayer> It(GetWorld()); It; ++It)
	{
		Hunters.Add(*It);
	}


	for (TActorIterator<ASlime_Control> It(GetWorld()); It; ++It)
	{
		Slime.Add(*It);
	}
}

void AGameManager::Tick(float deltatime)
{
	Super::Tick(deltatime);

	WorldTime += GetWorld()->GetDeltaSeconds();
	
	for (int i = 0; i < Hunters.Num(); ++i) {
		//Cast<AVacuumPlayer>(Hunters[i])->GetPlayerHUD()->SetTime(WorldTime);
	}
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
