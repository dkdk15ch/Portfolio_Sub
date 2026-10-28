// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PaperZD_3D2DTarget : TargetRules
{
	public PaperZD_3D2DTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("PaperZD_3D2D");
		
		// FastBuild設定 (UE 5.6)
		bUseUnityBuild = false; // FastBuildとUnityBuildは競合する可能性があるため無効化
		// Note: UE 5.6ではbUseFastBuildプロパティは削除されており、
		// FastBuildはデフォルトで有効化される条件が整っていれば自動的に使用される
	}
}
