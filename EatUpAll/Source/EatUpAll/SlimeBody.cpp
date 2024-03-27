// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeBody.h"

// Sets default values
ASlimeBody::ASlimeBody()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASlimeBody::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlimeBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlimeBody::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

