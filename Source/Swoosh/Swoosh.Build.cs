// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Swoosh : ModuleRules
{
	public Swoosh(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "AIModule", "GameplayTasks","NavigationSystem", "LevelSequence", "MovieScene" });
	}
}
