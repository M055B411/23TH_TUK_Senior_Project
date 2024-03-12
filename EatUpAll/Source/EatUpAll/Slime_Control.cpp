// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime_Control.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"


// Sets default values
ASlime_Control::ASlime_Control()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SlimeCore = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("CoreBody"));
			RootComponent = SlimeCore;
	if (SlimeCore)
	{
		SlimeCore->AlwaysLoadOnClient = true;
		SlimeCore->AlwaysLoadOnServer = true;
		SlimeCore->bOwnerNoSee = false;
		SlimeCore->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		SlimeCore->bCastDynamicShadow = true;
		SlimeCore->bAffectDynamicIndirectLighting = true;
		SlimeCore->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		static FName MeshCollisionProfileName(TEXT("Slime"));
		SlimeCore->SetCollisionProfileName(MeshCollisionProfileName);
		SlimeCore->SetGenerateOverlapEvents(true);
		SlimeCore->SetCanEverAffectNavigation(false);

	}

	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
	}

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}


// Called when the game starts or when spawned
void ASlime_Control::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlime_Control::Move(const FInputActionValue& Value)
{
	FVector2D MoveAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		SetMoveRight(MoveAxisVector.X);
		SetMoveForward(MoveAxisVector.Y);
	}
}

void ASlime_Control::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Called every frame
void ASlime_Control::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlime_Control::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASlime_Control::TraceJumpPath);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASlime_Control::SlimeJump);
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlime_Control::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlime_Control::Look);

	}
}

void ASlime_Control::GetMovementDirection()
{
}

void ASlime_Control::TraceMovement()
{
}

void ASlime_Control::TraceFloor()
{
}

void ASlime_Control::SlimeMove()
{
}

void ASlime_Control::IdentifyVacuumables()
{
}

void ASlime_Control::TraceJumpPath()
{
}

float ASlime_Control::GetJumpChargeTime(float DeltaTime)
{
	return 0.0f;
}

void ASlime_Control::SlimeJump()
{
}

