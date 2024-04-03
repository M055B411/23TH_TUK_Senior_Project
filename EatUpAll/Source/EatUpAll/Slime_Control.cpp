// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime_Control.h"
#include "SlimeController.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


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
	TargetLocation = this->GetActorLocation();
}

void ASlime_Control::Move(const FInputActionValue& Value)
{
	FVector2D MoveAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr) 
	{
		SetMoveForward(MoveAxisVector.Y);
		SetMoveRight(MoveAxisVector.X);
		
	}
}

void ASlime_Control::EndMove(const FInputActionValue& Value)
{
	FVector2D MoveAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		SetMoveForward(MoveAxisVector.Y);
		SetMoveRight(MoveAxisVector.X);

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
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

// Called every frame
void ASlime_Control::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceMovement();
	SlimeMove();
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("INPUT: %f,%f"), GetMoveRight(), GetMoveForward()));
}

// Called to bind functionality to input
void ASlime_Control::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ASlimeController* AC = GetController<ASlimeController>();
	check(EC&&AC);
	EC->BindAction(AC->MoveAction, ETriggerEvent::Triggered, this, &ASlime_Control::Move);
	EC->BindAction(AC->MoveAction, ETriggerEvent::Completed, this, &ASlime_Control::EndMove);
	EC->BindAction(AC->JumpAction, ETriggerEvent::Triggered, this, &ASlime_Control::TraceJumpPath);
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
	FVector Result = (FVector)(0,0,0);
	float angle = GetCoreBody()->GetComponentRotation().Roll;
	float Roll = GetControlRotation().Roll;
	float Pitch = GetControlRotation().Pitch;
	float Yaw = GetControlRotation().Yaw;
	Result = UKismetMathLibrary::Normal((UKismetMathLibrary::GetForwardVector((FRotator)(0,0,Yaw))
		* GetMoveForward())
		+ (UKismetMathLibrary::GetRightVector((FRotator)(Roll, 0, Yaw))
			* GetMoveRight()));
	if (angle <= 45) {
		
	}
	else if (angle <= 135) {

	}
	else 
	{

	}
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("DIR: %f,%f,%f"), Result.X, Result.Y, Result.Z));
	return Result;
}

void ASlime_Control::TraceMovement()
{
	FHitResult ForwardHIt1, ForwardHit2, ForwardHit3;
	/*TraceFloor(ForwardHIt1, ForwardHit2, ForwardHit3);*/
	FVector StartLocation = this->GetActorLocation();
	FVector Direction = GetMovementDirection();
	FVector EndLocation = (FVector)((UKismetMathLibrary::Normal((Direction * 1000.f) + (this->GetActorUpVector() * -500.f))
		* PathTraceLength) + StartLocation);
	
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Start: %f,%f,%f"), StartLocation.X, StartLocation.Y, StartLocation.Z));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("DIr to END : %f,%f,%f"), Direction.X, Direction.Y, Direction.Z));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("END: %f,%f,%f"), EndLocation.X, EndLocation.Y, EndLocation.Z));
	
	FHitResult OutHit;
	TArray<AActor*> ActorstoIgnore;
	ActorstoIgnore.Add(this);
	if (UKismetSystemLibrary::LineTraceSingle(this, StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorstoIgnore, EDrawDebugTrace::None, OutHit, true))
	{
		ForwardHIt1 = OutHit;
		EndLocation = (FVector)((UKismetMathLibrary::Normal((Direction * 1100.f) + (this->GetActorUpVector() * -500.f))
			* PathTraceLength) + StartLocation);
		
		if (UKismetSystemLibrary::LineTraceSingle(this, StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorstoIgnore, EDrawDebugTrace::ForDuration, OutHit, true)) {
			ForwardHit2 = OutHit;
			EndLocation = (FVector)((UKismetMathLibrary::Normal(
				(Direction * 1000.f) + (this->GetActorUpVector() * -500.f)
				+ (UKismetMathLibrary::RotateAngleAxis(Direction, 90.f, this->GetActorUpVector()) * 100.f)) * PathTraceLength) + StartLocation);
			
			if (UKismetSystemLibrary::LineTraceSingle(this, StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorstoIgnore, EDrawDebugTrace::None, OutHit, true)) {
				ForwardHit3 = OutHit;
				
			}
		}
	}
	
	TargetLocation = (FVector)(ForwardHIt1.Location + (ForwardHIt1.Normal * HalfHeight));
	TargetRotation = UKismetMathLibrary::MakeRotationFromAxes(UKismetMathLibrary::GetDirectionUnitVector(ForwardHIt1.Location, ForwardHit2.Location), UKismetMathLibrary::GetDirectionUnitVector(ForwardHIt1.Location, ForwardHit3.Location), ForwardHIt1.Normal);
}

void ASlime_Control::TraceFloor(FHitResult& ForwardHit1, FHitResult& ForwardHit2, FHitResult& ForwardHit3)
{
	FVector StartLocation = this->GetActorLocation();
	FVector EndLocation = (FVector)((UKismetMathLibrary::Normal((GetMovementDirection() * 100.f) + (this->GetActorUpVector() * -50.f))
		* PathTraceLength) + StartLocation);
	FHitResult OutHit;
	if(GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility))
	{
		ForwardHit1 = OutHit;
		EndLocation = (FVector)((UKismetMathLibrary::Normal((GetMovementDirection() * 110.f) + (this->GetActorUpVector() * -50.f))
			* PathTraceLength) + StartLocation);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility)) {
			ForwardHit2 = OutHit;
			EndLocation = (FVector)((UKismetMathLibrary::Normal((GetMovementDirection() * 110.f) + (this->GetActorUpVector() * -50.f) 
				+ (UKismetMathLibrary::RotateAngleAxis(StartLocation,90.f,EndLocation)*10.f)) * PathTraceLength) + StartLocation);
			if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility)) {
				ForwardHit3 = OutHit;
			}
		}
	}
	DrawDebugLine(
		GetWorld(),
		StartLocation,
		ForwardHit1.Location,
		FColor(255, 0, 0),
		false, -1, 0,
		1.333
	);

}

void ASlime_Control::SlimeMove()
{
	if((UKismetMathLibrary::Abs(GetMoveForward()) + UKismetMathLibrary::Abs(GetMoveRight())) > 0 && UKismetMathLibrary::Vector_Distance(TargetLocation,this->GetActorLocation())> 2)
	{
		FVector direction = UKismetMathLibrary::GetDirectionUnitVector(this->GetActorLocation(), TargetLocation);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("DIrection : %f,%f,%f"), direction.X, direction.Y, direction.Z));
		this->AddMovementInput(direction,MoveScale);
		this->K2_SetActorRotation(UKismetMathLibrary::RLerp(this->GetActorRotation(), TargetRotation,0.05, true),false);
	}
}

void ASlime_Control::IdentifyVacuumables()
{
}

void ASlime_Control::TraceJumpPath(const FInputActionValue& Value)
{
	bool val = Value.Get<bool>();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), val));
}

float ASlime_Control::GetJumpChargeTime(float DeltaTime)
{
	return 0.0f;
}

void ASlime_Control::SlimeJump(const FInputActionValue& Value)
{
}

void ASlime_Control::ParticleToggle()
{
}

