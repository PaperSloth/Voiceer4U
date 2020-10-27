// Copyright(c) 2020 PaperSloth

using UnrealBuildTool;

public class Voiceer4U : ModuleRules
{
    public Voiceer4U(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { });
        PrivateIncludePaths.AddRange(new string[] { });

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "UnrealEd",
            }
            );

        DynamicallyLoadedModuleNames.AddRange(new string[] { });
    }
}
