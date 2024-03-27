// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Slime_Control.generated.h"

class UArrowComponent;

UCLASS()
class EATUPALL_API ASlime_Control : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASlime_Control();

	class USkeletalMeshComponent* GetCoreBody() { return SlimeCore; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	class UArrowComponent* GetArrowComponent() const { return ArrowComponent;}
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SlimeCore;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
		float MaxHP;

	UPROPERTY()
		class UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAcess = "true"))
		class UFloatingPawnMovement* FloatMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAcess = "true"))
		float MoveScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MoveRight_var;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MoveForward_var;

	/** Called for movement input */
	void Move(const struct FInputActionValue& Value);
	void EndMove(const struct FInputActionValue& Value);

	/** Called for looking input */
	void Look(const struct FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FRotator TargetRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float HalfHeight = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float PathTraceLength = 50.f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetMovementDirection();
	
	void TraceMovement();

	void TraceFloor(FHitResult& ForwardHit1, FHitResult& ForwardHit2, FHitResult& ForwardHit3);

	void SlimeMove();

	void IdentifyVacuumables();

	void TraceJumpPath(const FInputActionValue& Value);

	float GetJumpChargeTime(float DeltaTime);

	void SlimeJump(const FInputActionValue& Value);

	void SetMoveRight(float value) { MoveRight_var = value; }
	float GetMoveRight() { return MoveRight_var; }

	void SetMoveForward(float value) { MoveForward_var = value; }
	float GetMoveForward() { return MoveForward_var; }

	void ParticleToggle();


};
