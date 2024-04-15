// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void UHunterWidget::SetCharge(float CurrentCharge, float MaxCharge)
{
	if (ChargeBar) {
		ChargeBar->SetPercent(CurrentCharge / MaxCharge);
	}
}

void UHunterWidget::SetCapacity(float CurrentCap, float MaxCap)
{
	if (CapacityBar) {
		CapacityBar->SetPercent(CurrentCap / MaxCap);
	}
}

void UHunterWidget::SetTime(float vTime)
{
	if (Time) {
		Time->SetText(UKismetTextLibrary::Conv_StringToText(UKismetStringLibrary::TimeSecondsToString(vTime)));
	}
	
}
	
