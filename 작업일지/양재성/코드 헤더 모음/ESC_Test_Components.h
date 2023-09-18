// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComponentWrapper.h"
#include "ECS_Test/Database.h"
#include "ESC_Test_Components.generated.h"


USTRUCT(BlueprintType)
struct FSlimeBubbleComp {
	GENERATED_BODY()
		FSlimeBubbleComp() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool Isattached = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float bubblehealth;
};

//struct FSlimeCoreComp {
//	GENERATED_BODY()
//		FSlimeCoreComp() {}
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//		float Corehealth;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//		float Charhealth;
//};

USTRUCT(BlueprintType)
struct FActorReference {
	GENERATED_BODY()
		FActorReference(AActor* Owner = nullptr) { Ptr = Owner; }

	TWeakObjectPtr<AActor> Ptr;

};
USTRUCT(BlueprintType)
struct FActorTransform {
	GENERATED_BODY()
		FActorTransform() {};
	FActorTransform(FTransform trns) : transform(trns) {};

	UPROPERTY(EditAnywhere, Category = ECS)
		FTransform transform;
};
USTRUCT(BlueprintType)
struct FCopyTransformToECS {
	GENERATED_BODY()
};
USTRUCT(BlueprintType)
struct FCopyTransformToActor {
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class ETransformSyncType : uint8 {
	ECS_To_Actor,
	Actor_To_ECS,
	BothWays,
	Disabled

};

UCLASS(ClassGroup = (ECS), meta = (BlueprintSpawnableComponent))
class SLIME_TEST_API UECSLinker : public UActorComponent
{
	GENERATED_BODY()

public:
	UECSLinker();

protected:
	virtual void BeginPlay() override;

	void RegisterWithECS();
	int32 EntityId = -1;
	class AECS_Manager_test* ECSManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "ECS")
		ETransformSyncType TransformSync = ETransformSyncType::BothWays;
};

UCLASS(ClassGroup = (ECS), meta = (BlueprintSpawnableComponent))
class SLIME_TEST_API USlimebubblewrapper : public UActorComponent, public IComponentWrapper
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USlimebubblewrapper() { PrimaryComponentTick.bCanEverTick = false; };
protected:
	void AddToEntity(Database* DB, int32 EntityId) final
	{
		DB->AddComp(EntityId, FSlimeBubbleComp());
	}
};