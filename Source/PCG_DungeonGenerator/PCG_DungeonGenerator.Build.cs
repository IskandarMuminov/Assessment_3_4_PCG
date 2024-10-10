// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PCG_DungeonGenerator : ModuleRules
{
	public PCG_DungeonGenerator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
