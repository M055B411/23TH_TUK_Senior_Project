// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeCharacter.h"

// Sets default values
ASlimeCharacter::ASlimeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASlimeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlimeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlimeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

