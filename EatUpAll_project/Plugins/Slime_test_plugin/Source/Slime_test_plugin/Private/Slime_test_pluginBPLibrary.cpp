// Copyright Epic Games, Inc. All Rights Reserved.

#include "Slime_test_pluginBPLibrary.h"
#include "Slime_test_plugin.h"

USlime_test_pluginBPLibrary::USlime_test_pluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float USlime_test_pluginBPLibrary::Slime_test_pluginSampleFunction(float Param)
{
	return -1;
}

