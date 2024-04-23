// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectAbyss : ModuleRules
{
    public ProjectAbyss(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateIncludePaths.AddRange(new string[] {
            "ProjectAbyss/Actors",
            "ProjectAbyss/Components",
            "ProjectAbyss/Enums",
            "ProjectAbyss/Gamemodes",
            "ProjectAbyss/Managers",
            "ProjectAbyss/Subsystems",
            "ProjectAbyss/Controllers",
            "ProjectAbyss/GameInstances",
            "ProjectAbyss/GameStates",
            "ProjectAbyss/Actors/Pawns",
            "ProjectAbyss/Actors/MissionSystem",
            "ProjectAbyss/Actors/Equipment",
            "ProjectAbyss/Widgets",
            "ProjectAbyss/GameInstance_GameSave",
            "ProjectAbyss",
        });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "GameFrwkSessionsPlugin", "OnlineSubSystem", "OnlineSubsystemUtils", "UMG", "Niagara", "MediaAssets" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        //SESSIONS_INCLUDE
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    }
}

