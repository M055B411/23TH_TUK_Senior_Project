// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
    auto GetRoleString = [](ENetRole Role) -> FString
    {
        switch (Role)
        {
        case ENetRole::ROLE_Authority:
            return TEXT("Authority");
        case ENetRole::ROLE_AutonomousProxy:
            return TEXT("AutonomousProxy");
        case ENetRole::ROLE_SimulatedProxy:
            return TEXT("SimulatedProxy");
        case ENetRole::ROLE_None:
            return TEXT("None");
        default:
            return TEXT("Unknown");
        }
    };

    FString LRole = GetRoleString(InPawn->GetLocalRole());
    FString RRole = GetRoleString(InPawn->GetRemoteRole());

    FString PlayerName;
    if (APlayerState* PlayerState = InPawn->GetPlayerState())
    {
        PlayerName = PlayerState->GetPlayerName();
    }

    FString RoleString = FString::Printf(TEXT("Local Role: %s\nRemote Role: %s\nName: %s"), *LRole, *RRole, *PlayerName);

    SetDisplayText(RoleString);
}

void UOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::NativeDestruct();
}
