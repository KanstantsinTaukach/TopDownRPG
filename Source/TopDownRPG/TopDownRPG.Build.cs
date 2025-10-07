// Copyright K.Taukach

using UnrealBuildTool;

public class TopDownRPG : ModuleRules
{
	public TopDownRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "UMG" }); 

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "GameplayTasks" });
		
		PublicIncludePaths.AddRange(new string[]
		{
			"TopDownRPG/Public/Player",
			"TopDownRPG/Public/Character",
			"TopDownRPG/Public/Game",
			"TopDownRPG/Public/Interaction",
			"TopDownRPG/Public/AbilitySystem",
			"TopDownRPG/Public/Actor",
			"TopDownRPG/Public/UI",
			"TopDownRPG/Public/UI/Widget",
			"TopDownRPG/Public/UI/WidgetController",
			"TopDownRPG/Public/UI/HUD"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
