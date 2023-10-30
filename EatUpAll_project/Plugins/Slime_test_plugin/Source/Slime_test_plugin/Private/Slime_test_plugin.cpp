// Copyright Epic Games, Inc. All Rights Reserved.

#include "Slime_test_plugin.h"

#define LOCTEXT_NAMESPACE "FSlime_test_pluginModule"

void FSlime_test_pluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FSlime_test_pluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, Slime_test_plugin, "Slime_test_plugin")