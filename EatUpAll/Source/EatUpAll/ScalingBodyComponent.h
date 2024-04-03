// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScalingBodyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EATUPALL_API UScalingBodyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScalingBodyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void detectEdible();

	UStaticMeshComponent* GetBodyMesh() { return Mesh; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Body, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
		float HP;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
		FVector InitialScale;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
		FVector MaxScale;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
		TArray< TEnumAsByte< EObjectTypeQuery > > Edible;

};
