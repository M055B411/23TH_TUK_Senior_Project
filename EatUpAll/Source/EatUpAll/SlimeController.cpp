// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

	//// Create these objects here and not in constructor because we only need them on the client.
	//MappingContext = NewObject<UInputMappingContext>(this);

	//MoveAction = NewObject<UInputAction>(this);
	//MoveAction->ValueType = EInputActionValueType::Axis2D;
	//MapKey(MappingContext, MoveAction, EKeys::W, false, true);
	//MapKey(MappingContext, MoveAction, EKeys::S, true, true);
	//MapKey(MappingContext, MoveAction, EKeys::A, true, false);
	//MapKey(MappingContext, MoveAction, EKeys::D, false, false);

	//LookAction = NewObject<UInputAction>(this);
	//LookAction->ValueType = EInputActionValueType::Axis2D;
	//MapKey(MappingContext, LookAction, EKeys::Mouse2D);

	//JumpAction = NewObject<UInputAction>(this);
	//JumpAction->ValueType = EInputActionValueType::Boolean;
	//MapKey(MappingContext, JumpAction, EKeys::SpaceBar, false, true, EInputAxisSwizzle::ZYX);
}

void ASlimeController::BeginPlay()
{
	Super::BeginPlay();
}

