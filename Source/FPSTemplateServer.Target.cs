// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSTemplateServerTarget : TargetRules
{
	public FPSTemplateServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server; // Type은 Server 로
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("FPSTemplate");
	}
}