// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime_Body.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASlime_Body::ASlime_Body()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMeshComponent"));
	BodyMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(BodyMesh);
}

// Called when the game starts or when spawned
void ASlime_Body::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASlime_Body::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckEdible();
	CarryandEat();
	
}

void ASlime_Body::CheckEdible()
{
}

void ASlime_Body::CarryandEat()
{
}

void ASlime_Body::ChangeBodyScale(float value)
{
	FVector CurScale = this->GetActorScale3D();

	HP -= value;

	this->SetActorScale3D((FVector)(CurScale * HP / MaxHP));
}

void ASlime_Body::ReceivePointDamage(float Damage, const UDamageType* DamageType, FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent, FName BoneName, FVector ShotFromDirection, AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo)
{
	ChangeBodyScale(Damage);
}

