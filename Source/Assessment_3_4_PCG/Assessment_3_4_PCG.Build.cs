// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Assessment_3_4_PCG : ModuleRules
{
	public Assessment_3_4_PCG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
