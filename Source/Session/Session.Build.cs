using UnrealBuildTool;

public class Session : ModuleRules
{
	public Session(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay",
			"UMG"
		});

		PublicIncludePaths.Add(ModuleDirectory);
	}
}
