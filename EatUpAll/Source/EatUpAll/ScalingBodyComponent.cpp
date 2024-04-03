// Fill out your copyright notice in the Description page of Project Settings.


#include "ScalingBodyComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UScalingBodyComponent::UScalingBodyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UScalingBodyComponent::BeginPlay()
{
	Super::BeginPlay();

	InitialScale = Mesh->GetComponentScale();
	MaxScale = (FVector)(InitialScale * 2.f);





	// ...
	
}


// Called every frame
void UScalingBodyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	detectEdible();
	// ...
}

void UScalingBodyComponent::detectEdible()
{
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorstoIgnore;
	if (UKismetSystemLibrary::SphereTraceMultiForObjects(this->GetOwner(), this->GetOwner()->GetActorLocation(), this->GetOwner()->GetActorLocation(),
		40.f, Edible, false, ActorstoIgnore, EDrawDebugTrace::ForDuration, OutHits, true, FLinearColor::Red, FLinearColor::Green, 0.f)) 
	{
		for (FHitResult Out : OutHits)
		{

		}
	}
}

