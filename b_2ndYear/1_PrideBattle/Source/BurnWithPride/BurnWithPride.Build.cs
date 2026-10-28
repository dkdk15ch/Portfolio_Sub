// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BurnWithPride : ModuleRules
{
	public BurnWithPride(ReadOnlyTargetRules Target) : base(Target)
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
            "Slate",
            "NavigationSystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "AssetRegistry"
        });

        PublicIncludePaths.AddRange(new string[] {
			"BurnWithPride",
			"BurnWithPride/Variant_Combat/Interfaces",
			"BurnWithPride/Enemy",
            "BurnWithPride/GravityController",
            "BurnWithPride/PathFinding",
            "BurnWithPride/StateTree",
            "BurnWithPride/Player",
            "BurnWithPride/Counter",
            "BurnWithPride/Bullet",
        });

		// Slate UIを使用する場合はコメントを外してください
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// オンライン機能を使用する場合はコメントを外してください
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// OnlineSubsystemSteamを使用する場合は、uprojectファイルのPluginsセクションにEnabledをtrueとして追加してください
	}
}
