// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LightBikesCS450 : ModuleRules
{
	public LightBikesCS450(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
