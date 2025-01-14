// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EatUpAll : ModuleRules
{
	public EatUpAll(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "OnlineSubsystemSteam", "OnlineSubsystem", "OnlineSubsystemNull", "UMG" });
	}
}
