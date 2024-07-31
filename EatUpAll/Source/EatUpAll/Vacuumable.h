// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VacuumInterface.h"
#include "Components/TimelineComponent.h"
#include "Vacuumable.generated.h"

class UCurveFloat;

UCLASS()
class EATUPALL_API AVacuumable : public AActor, public IVacuumInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVacuumable();

	virtual void Tick(float DeltaTime) override;
	/*VacuumInterface*/
	UPrimitiveComponent* GetVacuumableMesh_Implementation();
	void ShrinkDown_Implementation(AVacuumGun* VacuumGun);
	void GetFired_Implementation();

	bool GetAbsorbable() { return bisAbsorbable; }


	UFUNCTION()
		float GetWeight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void DisableVacuumability();

	UFUNCTION(BlueprintCallable)
		void EnableVacuumability();

	UFUNCTION()
		void ShrinkDownUpdate(float Alpha);

	UFUNCTION()
		void ShrinkDownFinished();

	UFUNCTION()
		void ExpandToNormalUpdate(float Alpha);

	UFUNCTION()
		void SetTimelineFunctions();

	UFUNCTION()
		void DisableOnHit();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DisableOnHit();
	void Server_DisableOnHit_Implementation();
	bool Server_DisableOnHit_Validate();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_DisableOnHit();
	void Multi_DisableOnHit_Implementation();

	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vacuum")
		float Weight = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vacuum")
		float MinVelocityToDamageEnemy = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vacuum")
		float MaxVelocityToDisableHitEvent = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vacuum")
		float ReVacuumabilityResetTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
		float OnHitDisableTime = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vacuum")
		bool DisableTimerStarted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vacuum")
		AVacuumGun* TargetVacuumGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vacuum")
		FVector ShrinkStartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vacuum")
		FVector ShrinkStartScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vacuum")
		UCurveFloat* ShirnkDownCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vacuum")
		UCurveFloat* ExpandToNormalCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoreItem")
		bool bisPhaseItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
		bool bisAbsorbable;

	FTimeline ShrinkDownTimeline;

	FTimeline ExpandToNormalTimeline;

	FVector MeshOriginalSize;
};
