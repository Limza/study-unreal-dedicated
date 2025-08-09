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
			"GameLiftServerSDK"
		]);

		PrivateDependencyModuleNames.AddRange([
			"Slate", 
			"SlateCore"
		]);
	}
}
