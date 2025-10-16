// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ZajUE5 : ModuleRules
{
	public ZajUE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ZajUE5",
			"ZajUE5/Variant_Platforming",
			"ZajUE5/Variant_Platforming/Animation",
			"ZajUE5/Variant_Combat",
			"ZajUE5/Variant_Combat/AI",
			"ZajUE5/Variant_Combat/Animation",
			"ZajUE5/Variant_Combat/Gameplay",
			"ZajUE5/Variant_Combat/Interfaces",
			"ZajUE5/Variant_Combat/UI",
			"ZajUE5/Variant_SideScrolling",
			"ZajUE5/Variant_SideScrolling/AI",
			"ZajUE5/Variant_SideScrolling/Gameplay",
			"ZajUE5/Variant_SideScrolling/Interfaces",
			"ZajUE5/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
