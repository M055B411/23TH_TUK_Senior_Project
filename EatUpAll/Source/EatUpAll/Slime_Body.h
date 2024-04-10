// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slime_Body.generated.h"


UCLASS()
class EATUPALL_API ASlime_Body : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlime_Body();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vacuum")
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vacuum")
		TArray<AActor*> ActorsToIgnore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vacuum")
		TArray<FHitResult> EdibleHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vacuum")
		TArray<FHitResult> AttackHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP")
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
		float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP")
		float OverHeal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP")
		float MaxOverHeal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stage")
		bool bPhaseTwo;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckEdible();

	void CarryandEat();

	void ChangeBodyScale(float value);

	void ReceivePointDamage(float Damage, const UDamageType* DamageType, FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent, FName BoneName, FVector ShotFromDirection, AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo);
};
