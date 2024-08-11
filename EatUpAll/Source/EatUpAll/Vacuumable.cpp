// Fill out your copyright notice in the Description page of Project Settings.


#include "Vacuumable.h"
#include "VacuumGun.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Hunter_Controller.h"

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
	// SetTimelineFunctions();
}

void AVacuumable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// ShrinkDownTimeline.TickTimeline(DeltaTime);
	// ExpandToNormalTimeline.TickTimeline(DeltaTime);
}

UPrimitiveComponent* AVacuumable::GetVacuumableMesh_Implementation()
{
	return Mesh;
}

//void AVacuumable::ShrinkDown_Implementation(AVacuumGun* VacuumGun)
//{
//	UE_LOG(LogTemp, Warning, TEXT("333333333333333333333"));
//
//	// 서버에서만 타임라인을 시작하고, 클라이언트로 이를 동기화함
//	if (HasAuthority())
//	{
//		Multi_ShrinkDownTimeline(VacuumGun);
//		
//	}
//	else {
//		UE_LOG(LogTemp, Warning, TEXT("444444444444444444"));
//		Server_ShrinkDownTimeline(VacuumGun);
//	}
//}
//
//void AVacuumable::Server_ShrinkDownTimeline_Implementation(AVacuumGun* VacuumGun)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Server_ShrinkDownTimeline"));
//	Multi_ShrinkDownTimeline(VacuumGun);
//}
//
//bool AVacuumable::Server_ShrinkDownTimeline_Validate(AVacuumGun* VacuumGun)
//{
//	return true;
//}
//
//void AVacuumable::Multi_ShrinkDownTimeline_Implementation(AVacuumGun* VacuumGun)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Multi_ShrinkDownTimeline"));
//
//	ShrinkStartLocation = GetActorLocation();
//	ShrinkStartScale = GetActorScale3D();
//	Mesh->SetSimulatePhysics(false);
//	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	ShrinkDownTimeline.PlayFromStart();
//	TargetVacuumGun = VacuumGun;
//}
//
//void AVacuumable::ShrinkDownUpdate(float Alpha)
//{
//	if (HasAuthority())
//	{
//		Multi_ShrinkDownUpdate(Alpha);
//	}
//	else 
//	{
//		Server_ShrinkDownUpdate(Alpha);
//	}
//}
//
//void AVacuumable::Server_ShrinkDownUpdate_Implementation(float Alpha) 
//{
//	UE_LOG(LogTemp, Warning, TEXT("Server_ShrinkDownUpdate"));
//
//	Multi_ShrinkDownUpdate(Alpha);
//}
//
//bool AVacuumable::Server_ShrinkDownUpdate_Validate(float Alpha)
//{
//	return true;
//}
//
//void AVacuumable::Multi_ShrinkDownUpdate_Implementation(float Alpha)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Multi_ShrinkDownUpdate"));
//
//	FVector DesiredScale = UKismetMathLibrary::VLerp(ShrinkStartScale, FVector(0.f), Alpha);
//	SetActorScale3D(DesiredScale);
//	if (TargetVacuumGun)
//	{
//		FVector DesiredLocation = UKismetMathLibrary::VLerp(ShrinkStartLocation, TargetVacuumGun->GetMuzzleLocation(), Alpha);
//		SetActorLocation(DesiredLocation);
//	}
//}
//
//void AVacuumable::ShrinkDownFinished()
//{
//	if(HasAuthority())
//	{
//		Multi_ShrinkDownFinished();
//	}
//	else {
//		Server_ShrinkDownFinished();
//	}
//}
//
//void AVacuumable::Server_ShrinkDownFinished_Implementation()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Server_ShrinkDownFinished"));
//
//	Multi_ShrinkDownFinished();
//}
//
//bool AVacuumable::Server_ShrinkDownFinished_Validate()
//{
//	return true;
//}
//
//void AVacuumable::Multi_ShrinkDownFinished_Implementation()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Multi_ShrinkDownFinished"));
//
//	IVacuumInterface* VacuumGun = Cast<IVacuumInterface>(TargetVacuumGun);
//	if (VacuumGun)
//	{
//		VacuumGun->Execute_AddToAmmo(TargetVacuumGun, this);
//	}
//
//	// 오브젝트를 축소 과정이 끝나면 파괴
//	Destroy();
//}


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

//void AVacuumable::ExpandToNormalUpdate(float Alpha)
//{
//	FVector DesiredScale = UKismetMathLibrary::VLerp(FVector(0.2f), MeshOriginalSize, Alpha);
//	SetActorScale3D(DesiredScale);
//}

//void AVacuumable::SetTimelineFunctions()
//{
//	FOnTimelineFloat ShrinkProgressUpdate;
//	ShrinkProgressUpdate.BindUFunction(this, FName("ShrinkDownUpdate"));
//
//	FOnTimelineEvent ShrinkFinishedEvent;
//	ShrinkFinishedEvent.BindUFunction(this, FName("ShrinkDownFinished"));
//
//	if (ShirnkDownCurve)
//	{
//		ShrinkDownTimeline.AddInterpFloat(ShirnkDownCurve, ShrinkProgressUpdate);
//	}
//
//	ShrinkDownTimeline.SetTimelineFinishedFunc(ShrinkFinishedEvent);
//
//	FOnTimelineFloat ExpandProgressUpdate;
//	ExpandProgressUpdate.BindUFunction(this, FName("ExpandToNormalUpdate"));
//
//	if (ExpandToNormalCurve)
//	{
//		ExpandToNormalTimeline.AddInterpFloat(ExpandToNormalCurve, ExpandProgressUpdate);
//	}
//}

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