// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"

static void MapKey(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
	bool bNegate = false,
	bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{
	FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
	UObject* Outer = InputMappingContext->GetOuter();

	if (bNegate)
	{
		UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
		Mapping.Modifiers.Add(Negate);
	}

	if (bSwizzle)
	{
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
		Swizzle->Order = SwizzleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}

void ASlimeController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Create these objects here and not in constructor because we only need them on the client.
	MappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;
	MapKey(MappingContext, MoveAction, EKeys::W);
	MapKey(MappingContext, MoveAction, EKeys::S, true);
	MapKey(MappingContext, MoveAction, EKeys::A, true, true);
	MapKey(MappingContext, MoveAction, EKeys::D, false, true);

	LookAction = NewObject<UInputAction>(this);
	LookAction->ValueType = EInputActionValueType::Axis3D;
	MapKey(MappingContext, LookAction, EKeys::MouseY);
	MapKey(MappingContext, LookAction, EKeys::MouseX, false, true);

	JumpAction = NewObject<UInputAction>(this);
	JumpAction->ValueType = EInputActionValueType::Axis1D;
	MapKey(MappingContext, JumpAction, EKeys::SpaceBar, false, true, EInputAxisSwizzle::ZYX);
}
