// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime_Control.h"
#include "SlimeController.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASlime_Control::ASlime_Control()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SlimeCore = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("CoreBody"));
	RootComponent = SlimeCore;
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

	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->ArrowColor = FColor(150, 200, 255);
	ArrowComponent->bTreatAsASprite = true;
	ArrowComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ArrowComponent->bIsScreenSizeScaled = true;
	ArrowComponent->SetSimulatePhysics(false);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FloatMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloationgPawnMovement"));
	FloatMovement->MaxSpeed = 1200.f;
	FloatMovement->Acceleration = 600.f;
	FloatMovement->Deceleration = 1200.f;

	MoveScale = 1.f;
}


// Called when the game starts or when spawned
void ASlime_Control::BeginPlay()
{
	Super::BeginPlay();
	ASlimeController* AC = GetController<ASlimeController>();
	check(AC);
}

void ASlime_Control::Move(const FInputActionValue& Value)
{
	FVector2D MoveAxisVector = Value.Get<FVector2D>();

	TraceMovement();
	SlimeMove(MoveAxisVector);
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

	FVector location = this->GetActorLocation();


}

// Called to bind functionality to input
void ASlime_Control::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ASlimeController* AC = GetController<ASlimeController>();
	check(EC);
	check(AC);
	EC->BindAction(AC->MoveAction, ETriggerEvent::Triggered, this, &ASlime_Control::Move);
	EC->BindAction(AC->JumpAction, ETriggerEvent::Started, this, &ASlime_Control::TraceJumpPath);
	EC->BindAction(AC->JumpAction, ETriggerEvent::Completed, this, &ASlime_Control::SlimeJump);
	EC->BindAction(AC->LookAction, ETriggerEvent::Triggered, this, &ASlime_Control::Look);

	ULocalPlayer* LocalPlayer = AC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(AC->MappingContext, 0);
}

FVector ASlime_Control::GetMovementDirection()
{
	float angle = GetCoreBody()->GetComponentRotation().Roll;
	if (angle <= 45) {
		UKismetMathLibrary::GetForwardVector((FRotator)(0, 0, GetControlRotation().Yaw));
	}
	else if (angle <= 135) {

	}
	else {

	}
	return (FVector)(0, 0, 0);
}

void ASlime_Control::TraceMovement()
{
}

void ASlime_Control::TraceFloor()
{
}

void ASlime_Control::SlimeMove(FVector2D Axis)
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

void ASlime_Control::ParticleToggle()
{
}

