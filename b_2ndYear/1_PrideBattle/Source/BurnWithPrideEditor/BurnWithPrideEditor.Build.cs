using UnrealBuildTool;

public class BurnWithPrideEditor : ModuleRules
{
    public BurnWithPrideEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AssetRegistry",
            "UnrealEd",   // エディタ専用
            "AssetTools",
            "ToolMenus",
            "Projects"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            // �K�v�Ȃ炳��ɒǉ�
        });
    }
}