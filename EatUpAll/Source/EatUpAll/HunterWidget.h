// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HunterWidget.generated.h"


/**
 * 
 */

UCLASS(Abstract)
class EATUPALL_API UHunterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetCharge(float CurrentCharge, float MaxCharge);

	void SetCapacity(float CurrentCap, float MaxCap);

	void SetTime(float Time);

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
		class UProgressBar* ChargeBar;

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
		class UProgressBar* CapacityBar;

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
		class UTextBlock* Time;

};
