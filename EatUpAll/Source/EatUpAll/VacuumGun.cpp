// Fill out your copyright notice in the Description page of Project Settings.


#include "VacuumGun.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Vacuumable.h"
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
	if (HasAuthority())
	{
		// 서버에서 로직 실행
		Multi_Vacuum(DeltaTime);
	}
	else
	{
		// 클라이언트에서 서버로 요청
		Server_Vacuum(DeltaTime);
	}
}

void AVacuumGun::Server_Vacuum_Implementation(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_Vacuum_Implementation Has Been Called"));
	Multi_Vacuum(DeltaTime);
}

// Server Vacuum Validation
bool AVacuumGun::Server_Vacuum_Validate(float DeltaTime)
{
	// Add any validation logic here, return true if validation is successful
	return true;
}

void AVacuumGun::Multi_Vacuum_Implementation(float DeltaTime)
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
	float ForceMultiplier = 1000.f; // 힘의 크기를 조절하는 스칼라 값

	for (FHitResult HitResult : VacuumHitResultArray)
	{
		IVacuumInterface* HitVacuumable = Cast<IVacuumInterface>(HitResult.GetActor());
		if (HitVacuumable)
		{
			CurrentFrameHitActors.AddUnique(HitResult.GetActor());

			if (CanPlayerSeeThisObject(HitResult)&&Cast<AVacuumable>(HitResult.GetActor())->GetAbsorbable())
			{
				FVector ForceToAdd = GetForceToAdd(HitResult, DeltaTime) * ForceMultiplier;
				HitResult.GetComponent()->AddForce((FVector)(GetForceToAdd(HitResult, DeltaTime)), FName(""), true);
				// HitResult.GetComponent()->SetEnableGravity(false);

				if (CanAbsorbThisActor(HitResult))
				{
					Absorb(HitResult.GetActor());
				}
			}
		}
	}
}

void AVacuumGun::DamageTarget(float DeltaTime)
{
	if (HasAuthority())
	{
		// 서버에서 로직 실행
		Multi_DamageTarget(DeltaTime);
	}
	else
	{
		// 클라이언트에서 서버로 요청
		Server_DamageTarget(DeltaTime);
	}

	/*for (FHitResult HitResult : DamageHitResultArray)
	{
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 1.2f, nullptr, nullptr,  nullptr);
	}*/
}

void AVacuumGun::Server_DamageTarget_Implementation(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_DamageTarget_Implementation Has Been Called"));
	Multi_DamageTarget(DeltaTime);
}

bool AVacuumGun::Server_DamageTarget_Validate(float DeltaTime)
{
	// Add any validation logic here, return true if validation is successful
	return true;
}

void AVacuumGun::Multi_DamageTarget_Implementation(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Multi_DamageTarget_Implementation Has Been Called"));

	for (FHitResult HitResult : DamageHitResultArray)
	{
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 1.2f, nullptr, nullptr, nullptr);
		UE_LOG(LogTemp, Warning, TEXT("DamageTarget."));
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
	UE_LOG(LogTemp, Warning, TEXT("Absorb Has Been Called"));

	if (HasAuthority())
	{
		// 서버에서 로직 실행
		Multi_Absorb(HitActor);
	}
	else
	{
		// 클라이언트에서 서버로 요청
		Server_Absorb(HitActor);
	}

	//if (!CanFire() && IsVacuuming && OwnerInterface)
	//{
	//	IVacuumInterface* HitVacuumable = Cast<IVacuumInterface>(HitActor);
	//	if (HitVacuumable && Cast<AVacuumable>(HitActor)->GetWeight() < 1.f)
	//	{
	//		LastFrameHitActors.Remove(HitActor);
	//		CurrentFrameHitActors.Remove(HitActor);
	//		Execute_ShrinkDown(HitActor, this);
	//	}
	//}
}


void AVacuumGun::Server_Absorb_Implementation(AActor* HitActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_Absorb_Implementation Has Been Called"));
	Multi_Absorb(HitActor);
}

bool AVacuumGun::Server_Absorb_Validate(AActor* HitActor)
{
	// Add any validation logic here, return true if validation is successful
	return true;
}

void AVacuumGun::Multi_Absorb_Implementation(AActor* HitActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Multi_Absorb_Implementation Has Been Called"));

	if (!CanFire() && IsVacuuming && OwnerInterface)
	{
		IVacuumInterface* HitVacuumable = Cast<IVacuumInterface>(HitActor);
		if (HitVacuumable && Cast<AVacuumable>(HitActor)->GetWeight() < 1.f)
		{
			LastFrameHitActors.Remove(HitActor);
			CurrentFrameHitActors.Remove(HitActor);
			Execute_ShrinkDown(HitActor, this);
		}
	}
}

bool AVacuumGun::CanAbsorbThisActor(FHitResult HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("CanAbsorbThisActor Has Been Called"));

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

//void AVacuumGun::FireAmmo()
//{
//	if (CanFire() && GetWorld())
//	{
//		
//		
//		if (ProjectileClass != nullptr) {
//			FVector LuanchDirection = (GetTraceEndLocation() - GetTraceStartLocation()).GetSafeNormal();
//
//			
//			FActorSpawnParameters ActorSpawnParams;
//			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
//			const FRotator SpawnRotation = GetOwner()->GetActorRotation();
//
//			AVacuumProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AVacuumProjectile>(ProjectileClass, GetTraceStartLocation(), SpawnRotation, ActorSpawnParams);
//
//			PlayFireSound();
//
//			IsVacuuming = false;
//			Ammo = 0.f;
//		}
//		
//	}
//}

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


//void AVacuumGun::PlayFireSound()
//{
//	if (FireSound)
//	{
//		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
//	}
//}
//
//
void AVacuumGun::PlayAbsorbSound()
{
	if (PopSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PopSound, GetActorLocation());
	}
}
//
//void AVacuumGun::PlayVacuumstSound()
//{
//	if (Vacuumsoundstart)
//	{
//		UGameplayStatics::PlaySoundAtLocation(this, Vacuumsoundstart, GetActorLocation());
//	}
//}
//
//void AVacuumGun::PlayVacuumonSound()
//{
//	if (Vacuumsoundongoing)
//	{
//		UGameplayStatics::PlaySoundAtLocation(this, Vacuumsoundongoing, GetActorLocation());
//	}
//}
//
//void AVacuumGun::PlayVacuumendSound()
//{
//	if (Vacuumsoundend)
//	{
//		UGameplayStatics::PlaySoundAtLocation(this, Vacuumsoundend, GetActorLocation());
//	}
//}

//void AVacuumGun::CheckOverloaded()
//{
//	if (CurChargetime >= MaxChargeTime)
//	{
//		CurChargetime = MaxChargeTime;
//		bIsOverloaded = true;
//	}
//}
//
//void AVacuumGun::Cooldown()
//{
//	if(!IsVacuuming)
//	{
//		CurChargetime -= GetWorld()->GetDeltaSeconds();
//		if (CurChargetime < 0) {
//			CurChargetime = 0;
//			bIsOverloaded = false;
//		}
//	}
//}

FVector AVacuumGun::GetMuzzleLocation()
{
	return WeaponMesh->GetSocketLocation(FName("Muzzle"));
}