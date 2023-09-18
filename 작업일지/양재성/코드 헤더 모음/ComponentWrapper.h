// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentWrapper.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UComponentWrapper : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IComponentWrapper
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AddToEntity(class Database* DB, int32 EntitiyId) = 0;
};
