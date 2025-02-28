// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
public class BaseDefense : ModuleRules
{
	public BaseDefense(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableUndefinedIdentifierWarnings = false;
        //PrivatePCHHeaderFile = "BaseDefense.h";


        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "OnlineSubsystemSteam", "SlateCore", "OnlineSubsystem", "OnlineSubsystemUtils", "NavigationSystem", "AnimationSharing", "Steamworks", "AnimGraphRuntime"});


        PrivateDependencyModuleNames.AddRange(new string[] {  });

        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

        //DynamicallyLoadedModuleNames.AddRange(new string[] { "SimplexNoise" });


        PrivateIncludePaths.AddRange(
            new string[] {
                "BaseDefense/Actors",
                "BaseDefense/Components",
                "BaseDefense/Controllers",
                "BaseDefense/Interfaces",
                "BaseDefense/Main",
                "BaseDefense/Controllers",
                "BaseDefense/Pawns",
                "BaseDefense/UI",
                "BaseDefense/AI",
                "BaseDefense/Levels",
            }
        );



        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
