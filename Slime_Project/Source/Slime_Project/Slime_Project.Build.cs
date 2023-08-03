// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Slime_Project : ModuleRules
{
	public Slime_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
