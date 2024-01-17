// Fill out your copyright notice in the Description page of Project Settings.


#include "Vacuumable.h"
#include "VacuumGun.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AVacuumable::AVacuumable()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetSimulatePhysics(true);
	SetRootComponent(Mesh);
}

void AVacuumable::BeginPlay()
{
	Super::BeginPlay();
	MeshOriginalSize = GetActorScale();
	SetTimelineFunctions();
}

void AVacuumable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ShrinkDownTimeline.TickTimeline(DeltaTime);
	ExpandToNormalTimeline.TickTimeline(DeltaTime);
}

UPrimitiveComponent* AVacuumable::GetVacuumableMesh_Implementation()
{
	return Mesh;
}

void AVacuumable::ShrinkDown_Implementation(AVacuumGun* VacuumGun)
{
	ShrinkStartLocation = GetActorLocation();
	ShrinkStartScale = GetActorScale3D();
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShrinkDownTimeline.PlayFromStart();
	TargetVacuumGun = VacuumGun;
}

void AVacuumable::GetFired_Implementation()
{
	DisableVacuumability();
	Mesh->SetNotifyRigidBodyCollision(true);
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AVacuumable::EnableVacuumability, ReVacuumabilityResetTime);

}

void AVacuumable::DisableVacuumability()
{
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void AVacuumable::EnableVacuumability()
{
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
}

void AVacuumable::ShrinkDownUpdate(float Alpha)
{
	FVector DesiredScale = UKismetMathLibrary::VLerp(ShrinkStartScale, FVector(0.f), Alpha);
	SetActorScale3D(DesiredScale);
	if (TargetVacuumGun)
	{
		FVector DesiredLocation = UKismetMathLibrary::VLerp(ShrinkStartLocation, TargetVacuumGun->GetMuzzleLocation(), Alpha);
		SetActorLocation(DesiredLocation);
	}
}

void AVacuumable::ShrinkDownFinished()
{
	IVacuumInterface* VacuumGun = Cast<IVacuumInterface>(TargetVacuumGun);
	if (VacuumGun)
	{
		VacuumGun->Execute_AddToAmmo(TargetVacuumGun, this);
	}
}

void AVacuumable::ExpandToNormalUpdate(float Alpha)
{
	FVector DesiredScale = UKismetMathLibrary::VLerp(FVector(0.2f), MeshOriginalSize, Alpha);
	SetActorScale3D(DesiredScale);
}

void AVacuumable::SetTimelineFunctions()
{
	FOnTimelineFloat ShrinkProgressUpdate;
	ShrinkProgressUpdate.BindUFunction(this, FName("ShrinkDownUpdate"));

	FOnTimelineEvent ShrinkFinishedEvent;
	ShrinkFinishedEvent.BindUFunction(this, FName("ShrinkDownFinished"));

	if (ShirnkDownCurve)
	{
		ShrinkDownTimeline.AddInterpFloat(ShirnkDownCurve, ShrinkProgressUpdate);
	}

	ShrinkDownTimeline.SetTimelineFinishedFunc(ShrinkFinishedEvent);

	FOnTimelineFloat ExpandProgressUpdate;
	ExpandProgressUpdate.BindUFunction(this, FName("ExpandToNormalUpdate"));

	if (ExpandToNormalCurve)
	{
		ExpandToNormalTimeline.AddInterpFloat(ExpandToNormalCurve, ExpandProgressUpdate);
	}
}

void AVacuumable::DisableOnHit()
{
	if (!DisableTimerStarted)
	{
		DisableTimerStarted = true;
		Mesh->SetNotifyRigidBodyCollision(false);
		UE_LOG(LogTemp, Warning, TEXT("DisabledOnHit"));
	}
}

float AVacuumable::GetWeight()
{
	return Weight;
}

