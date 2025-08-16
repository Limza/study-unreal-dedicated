using UnrealBuildTool;

public class DedicatedServers : ModuleRules
{
	public DedicatedServers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange([
			"Core", 
			"CoreUObject", 
			"Engine",
			"GameLiftServerSDK",
			"GameplayTags",
			"HTTP"
		]);

		PrivateDependencyModuleNames.AddRange([
			"Slate", 
			"SlateCore",
			"UMG",
			"Json",
			"JsonUtilities"
		]);
	}
}
