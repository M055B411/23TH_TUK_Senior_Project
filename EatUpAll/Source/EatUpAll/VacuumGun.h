// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VacuumInterface.h"
#include "VacuumGun.generated.h"

class AVacuumable;
class UAmmoCount;

UCLASS()
class EATUPALL_API AVacuumGun : public AActor, public IVacuumInterface
{
	GENERATED_BODY()
	
public:	
	AVacuumGun();

	/*VacuumInterface*/
	void AddToAmmo_Implementation(AVacuumable* Vacuumable);

	void EnableVacuuming(bool ShouldVacuum);

	void CancelVacuumEffect();

	void FireAmmo();

	bool CanFire();

	UFUNCTION(BlueprintCallable)
	FVector GetMuzzleLocation();


	UFUNCTION(BlueprintCallable)
		float GetMaxCharge() { return MaxChargeTime; }
	UFUNCTION(BlueprintCallable)
		float GetCurCharge() { return CurChargetime; }

	UFUNCTION(BlueprintCallable)
		float GetAmmoCap() { return capacity; }
	UFUNCTION(BlueprintCallable)
		float GetCurAmmo() { return Ammo; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetOverloaded() { return bIsOverloaded; }

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetOwnerWithDelay();

	void GetOwnerInterface();

	void Vacuum(float DeltaTime);

	void TraceForVacuum();

	void PullAndAbsorb(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void Server_PullAndAbsorb(float DeltaTime);
	void Server_PullAndAbsorb_Implementation(float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PullAndAbsorb(float DeltaTime);
	void Multi_PullAndAbsorb_Implementation(float DeltaTime);

	bool CanPlayerSeeThisObject(FHitResult& HitResult);

	FVector GetForceToAdd(FHitResult& HitResult, float DeltaTime);

	void TraceForAbsorb();

	UFUNCTION(BlueprintCallable)
	void TraceForDamage();

	void Absorb(AActor* HitActor);

	bool CanAbsorbThisActor(FHitResult HitResult);

	void PlayFireSound();

	void PlayAbsorbSound();

	void PlayVacuumstSound();

	void PlayVacuumonSound();

	void PlayVacuumendSound();

	void CheckOverloaded();

	void Cooldown();

	FVector GetTraceStartLocation();

	FVector GetTraceEndLocation();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum", meta = (AllowPrivateAccess = "true"))
	float VacuumTraceStartOffset = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum", meta = (AllowPrivateAccess = "true"))
	float VacuumTraceRange = 2000.f;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AVacuumProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForVacuumTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForDamageTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForVisibiltyTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vacuum")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vacuum")
	TArray<FHitResult> VacuumHitResultArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vacuum")
	FHitResult VisibilityHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vacuum")
	TArray<FHitResult> AbsorbHitResultArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vacuum")
	TArray<AActor*> CurrentFrameHitActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vacuum")
	TArray<AActor*> LastFrameHitActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	bool ShouldDrawDebugForTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	float VacuumTraceRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	float VacuumForce = 200000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	float AbsorbWindow = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	float AbsorbRange = 300.f;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	bool IsVacuuming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	USoundBase* Vacuumsoundstart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	USoundBase* Vacuumsoundongoing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
	USoundBase* Vacuumsoundend;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float Ammo = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AmmoLaunchVelocity = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float capacity = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	USoundBase* PopSound;

	IVacuumInterface* OwnerInterface;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
		float MaxChargeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
		float CurChargetime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
		bool bIsOverloaded;


};
