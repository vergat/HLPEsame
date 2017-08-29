// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TimeAttack : ModuleRules
{
	public TimeAttack(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RenderCore", "PhysXVehicles", "HeadMountedDisplay" });
	}
}
