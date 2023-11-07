// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpaceInvaders : ModuleRules
{
	public SpaceInvaders(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG",
        "EnhancedInput", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "CommonUI" });
	}
}
