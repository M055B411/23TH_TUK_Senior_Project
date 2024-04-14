// Fill out your copyright notice in the Description page of Project Settings.


#include "VacuumPlayer.h"
#include "HunterWidget.h"
#include "Blueprint/UserWidget.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "VacuumGun.h"
#include "Hunter_Controller.h"

// Sets default values
AVacuumPlayer::AVacuumPlayer()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	PlayerHUDClass = nullptr;
	PlayerHUD = nullptr;
}

// Called when the game starts or when spawned
void AVacuumPlayer::BeginPlay()
{
	Super::BeginPlay();

	

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	EquipVacuumGun();
	
	if (IsLocallyControlled() && PlayerHUDClass)
	{
		AHunter_Controller* HC = GetController<AHunter_Controller>();
		check(HC);
		PlayerHUD = CreateWidget<UHunterWidget>(HC, PlayerHUDClass);
		check(PlayerHUD);
		PlayerHUD->AddToPlayerScreen();
		PlayerHUD->SetCharge(VacuumGun->GetCurCharge(), VacuumGun->GetMaxCharge());
		PlayerHUD->SetCapacity(VacuumGun->GetCurAmmo(), VacuumGun->GetAmmoCap());
	}
}

// Called to bind functionality to input
void AVacuumPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVacuumPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVacuumPlayer::Look);
		EnhancedInputComponent->BindAction(PerformVacuumAction, ETriggerEvent::Triggered, this, &AVacuumPlayer::PerformVacuum);
		EnhancedInputComponent->BindAction(StopPerformAction, ETriggerEvent::Triggered, this, &AVacuumPlayer::StopVacuum);
		EnhancedInputComponent->BindAction(PerformFireAction, ETriggerEvent::Triggered, this, &AVacuumPlayer::FireVacuumGun);
	}
}

void AVacuumPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AVacuumPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

UCameraComponent* AVacuumPlayer::GetPlayerCamera_Implementation()
{
	return FirstPersonCameraComponent;
	//->GetForwardVector() * VacuumTraceStartOffset + FirstPersonCameraComponent->GetComponentLocation();
}

void AVacuumPlayer::EquipVacuumGun()
{
	UWorld* World = GetWorld();
	if (!World || !VacuumGunClass) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	VacuumGun = World->SpawnActor<AVacuumGun>(VacuumGunClass);
	VacuumGun->SetOwner(this);
	VacuumGun->SetInstigator(this);
	if (this->IsLocallyControlled())
	{
		VacuumGun->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("LowerGrip")));
	}
	else 
	{
		VacuumGun->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("LowerGrip")));
	}
	//VacuumGun->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("UpperGrip")));
	bHasRifle = true;
}

void AVacuumPlayer::PerformVacuum()
{
		VacuumGun->EnableVacuuming(true);
}

void AVacuumPlayer::StopVacuum()
{
	VacuumGun->EnableVacuuming(false);
	VacuumGun->CancelVacuumEffect();
}

void AVacuumPlayer::FireVacuumGun()
{
	PlayFireMontage();
	VacuumGun->FireAmmo();
}

void AVacuumPlayer::PlayFireMontage()
{
	if (VacuumGun->CanFire())
	{
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance && FireMontage)
		{
			AnimInstance->Montage_Play(FireMontage);
		}
	}
}