// Fill out your copyright notice in the Description page of Project Settings.


#include "VacuumGun.h"
#include "Vacuumable.h"
#include "VacuumPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
// #include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "VacuumProjectile.h"

// Sets default values
AVacuumGun::AVacuumGun()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMeshComponent"));
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void AVacuumGun::BeginPlay()
{
	Super::BeginPlay();
	SetOwnerWithDelay();
	
}

// Called every frame
void AVacuumGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Vacuum(DeltaTime);
	//Cooldown();
}

void AVacuumGun::AddToAmmo_Implementation(AVacuumable* Vacuumable)
{
	if (Vacuumable)
	{
		Ammo++;
		
		if (Ammo > capacity) {
			Ammo = capacity;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), Ammo));
		// PlayAbsorbSound();
	}
}

void AVacuumGun::EnableVacuuming(bool ShouldVacuum)
{

	if (IsVacuuming != ShouldVacuum)
	{
		IsVacuuming = ShouldVacuum;
	}
}

void AVacuumGun::SetOwnerWithDelay()
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AVacuumGun::GetOwnerInterface);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.2f, false);
}

void AVacuumGun::GetOwnerInterface()
{
	if (GetOwner())
	{
		OwnerInterface = Cast<IVacuumInterface>(GetOwner());
	}
}

void AVacuumGun::Vacuum(float DeltaTime)
{
	if (IsVacuuming && OwnerInterface)
	{
		TraceForVacuum();
		TraceForDamage();

		//Pull & SetProperties of caught actors
		PullAndAbsorb(DeltaTime);
		DamageTarget(DeltaTime);

		//CurChargetime += GetWorld()->GetDeltaSeconds();
	}
	// if trace miss, set properties of the last frame hit actors
	CancelVacuumEffect();

	LastFrameHitActors = CurrentFrameHitActors;
	CurrentFrameHitActors.Empty();
}

void AVacuumGun::TraceForVacuum()
{
	FVector StartPoint = GetTraceStartLocation();
	FVector EndPoint = GetTraceEndLocation();
	UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartPoint, EndPoint, VacuumTraceRadius, ObjectTypesForVacuumTrace, false, ActorsToIgnore, ShouldDrawDebugForTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, VacuumHitResultArray, true, FLinearColor::Red, FLinearColor::Green, 0.0f);
}

void AVacuumGun::PullAndAbsorb(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("PullAndAbsorb Has Been Called"));

	float ForceMultiplier = 3.f; // 힘의 크기를 조절하는 스칼라 값

	for (FHitResult HitResult : VacuumHitResultArray)
	{
		IVacuumInterface* HitVacuumable = Cast<IVacuumInterface>(HitResult.GetActor());
		if (HitVacuumable)
		{
			CurrentFrameHitActors.AddUnique(HitResult.GetActor());

			if (CanPlayerSeeThisObject(HitResult)&&Cast<AVacuumable>(HitResult.GetActor())->GetAbsorbable())
			{
				FVector ForceToAdd = GetForceToAdd(HitResult, DeltaTime) * ForceMultiplier;

				// ForceToAdd 출력
				// UE_LOG(LogTemp, Warning, TEXT("Force To Add: %s"), *ForceToAdd.ToString());

				//HitResult.GetComponent()->AddForce((FVector)(GetForceToAdd(HitResult, DeltaTime)), FName(""), true);

				UPrimitiveComponent* HitComponent = HitResult.GetComponent();
				if (HitComponent)
				{
					// Force 적용
					HitComponent->AddForce(ForceToAdd, NAME_None, true);

					// 현재 속도 출력
					FVector CurrentVelocity = HitComponent->GetPhysicsLinearVelocity();
					// UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Current Velocity: %s"), *HitResult.GetActor()->GetName(), *CurrentVelocity.ToString());

					if (CanAbsorbThisActor(HitResult))
					{
						Absorb(HitResult.GetActor());
					}
				}
			}
		}
	}
}

void AVacuumGun::DamageTarget(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("DamageTarget Has Been Called"));

	for (FHitResult& HitResult : DamageHitResultArray)
	{
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 1.2f, nullptr, nullptr, nullptr);
	}
}

bool AVacuumGun::CanPlayerSeeThisObject(FHitResult& HitResult)
{
	UCameraComponent* PlayerCamera = Execute_GetPlayerCamera(GetOwner());
	UKismetSystemLibrary::LineTraceSingleForObjects(this, PlayerCamera->GetComponentLocation(), HitResult.ImpactPoint, ObjectTypesForVisibiltyTrace, false, ActorsToIgnore, ShouldDrawDebugForTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, VisibilityHitResult, true);
	return !VisibilityHitResult.bBlockingHit;
}

FVector AVacuumGun::GetForceToAdd(FHitResult& HitResult, float DeltaTime)
{
	FVector NormalizedToMuzzle = (WeaponMesh->GetSocketLocation(FName("Muzzle")) - HitResult.GetActor()->GetActorLocation()).GetSafeNormal();
	float distance = (WeaponMesh->GetSocketLocation(FName("Muzzle")) - HitResult.GetActor()->GetActorLocation()).Size() / 20.f;
	FVector VacuumForceVector = (NormalizedToMuzzle * VacuumForce) * DeltaTime * 1.2f / (Cast<AVacuumable>(HitResult.GetActor())->GetWeight()*distance);
	return VacuumForceVector;
}

void AVacuumGun::TraceForAbsorb()
{
	FVector TraceCenter = GetTraceStartLocation();
	UKismetSystemLibrary::SphereTraceMultiForObjects(this, TraceCenter, TraceCenter, VacuumTraceRadius, ObjectTypesForVacuumTrace, false, ActorsToIgnore, ShouldDrawDebugForTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, AbsorbHitResultArray, true, FLinearColor::Red, FLinearColor::Green, 0.0f);
}

void AVacuumGun::TraceForDamage()
{
	
	FVector StartPoint = GetTraceStartLocation();
	FVector EndPoint = GetTraceEndLocation();
	UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartPoint, EndPoint, VacuumTraceRadius, ObjectTypesForDamageTrace, false, ActorsToIgnore, ShouldDrawDebugForTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, DamageHitResultArray, true,FLinearColor::Red,FLinearColor::Green, 0.0f);
}

void AVacuumGun::Absorb(AActor* HitActor)
{
	if (IsVacuuming && OwnerInterface)	// (!CanFire() && IsVacuuming && OwnerInterface)
	{
		//IVacuumInterface* HitVacuumable = Cast<IVacuumInterface>(HitActor);
		//if (HitVacuumable && Cast<AVacuumable>(HitActor)->GetWeight() < 1.f)
		//{
		//	LastFrameHitActors.Remove(HitActor);
		//	CurrentFrameHitActors.Remove(HitActor);
		//	Execute_ShrinkDown(HitActor, this);
		//}

		if (Cast<AVacuumable>(HitActor)->GetWeight() < 0.8f)
		{
			LastFrameHitActors.Remove(HitActor);
			CurrentFrameHitActors.Remove(HitActor);

			if (AVacuumPlayer* VacuumPlayer = Cast<AVacuumPlayer>(GetOwner()))
            {
                // 블루프린트 노드에서 함수 호출
                VacuumPlayer->OnSuckAction(HitActor);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to cast to AVacuumPlayer"));
            }
		}
	}
}

bool AVacuumGun::CanAbsorbThisActor(FHitResult HitResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("CanAbsorbThisActor Has Been Called"));

	// 1. 흡수 범위 내에 있는지 확인
	if ((GetOwner()->GetActorLocation() - HitResult.ImpactPoint).Length() < AbsorbRange)
	{
		// 2. 타겟 위치와 카메라 위치를 기반으로 방향 벡터 계산
		FVector TargetLocation = HitResult.ImpactPoint;
		UCameraComponent* PlayerCamera = Execute_GetPlayerCamera(GetOwner());
		FVector PlayerCameraLocation = PlayerCamera->GetComponentLocation();
		FVector TargetDirection = (TargetLocation - PlayerCameraLocation).GetSafeNormal();
		FVector CameraForward = PlayerCamera->GetForwardVector();
		
		// 3. 카메라의 정면 벡터와 타겟 방향 벡터 사이의 각도 계산
		const double ChosTheta = FVector::DotProduct(CameraForward, TargetDirection);
		double Theta = FMath::Acos(ChosTheta);
		Theta = FMath::RadiansToDegrees(Theta);

		// 4. 카메라의 정면 벡터와 타겟 방향 벡터 사이의 각도가 흡수 창 안에 있는지 확인
		const FVector CrossProduct = FVector::CrossProduct(CameraForward, TargetDirection);
		if (Theta < AbsorbWindow && Theta > -AbsorbWindow)
		{
			return true;
		}
		return false;
	}
	return false;
}

bool AVacuumGun::CanFire()
{
	return Ammo == capacity;
}

void AVacuumGun::CancelVacuumEffect()
{
	for (AActor* LastHitInterface : LastFrameHitActors)
	{
		if (!CurrentFrameHitActors.Contains(LastHitInterface))
		{
			UPrimitiveComponent* VacuumableMesh = Execute_GetVacuumableMesh(LastHitInterface);
			VacuumableMesh->SetEnableGravity(true);
			VacuumableMesh->SetPhysicsLinearVelocity(VacuumableMesh->GetPhysicsLinearVelocity() / 3.f);
		}
	}
}

FVector AVacuumGun::GetTraceStartLocation()
{
	if (GetOwner())
	{
		UCameraComponent* PlayerCamera = Execute_GetPlayerCamera(GetOwner());
		if (PlayerCamera)
		{
			FVector StartPoint = PlayerCamera->GetForwardVector() * VacuumTraceStartOffset + PlayerCamera->GetComponentLocation();
			return StartPoint;
		}
	}
	return FVector(0.f);
}

FVector AVacuumGun::GetTraceEndLocation()
{
	if (GetOwner())
	{
		UCameraComponent* PlayerCamera = Execute_GetPlayerCamera(GetOwner());
		if (PlayerCamera)
		{
			FVector EndPoint = PlayerCamera->GetForwardVector() * VacuumTraceRange + PlayerCamera->GetComponentLocation();
			return EndPoint;
		}
	}
	return FVector(0.f);
}

void AVacuumGun::PlayAbsorbSound()
{
	if (PopSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PopSound, GetActorLocation());
	}
}

FVector AVacuumGun::GetMuzzleLocation()
{
	return WeaponMesh->GetSocketLocation(FName("Muzzle"));
}