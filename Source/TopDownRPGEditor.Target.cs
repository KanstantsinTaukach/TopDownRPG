// Copyright K.Taukach

using UnrealBuildTool;
using System.Collections.Generic;

public class TopDownRPGEditorTarget : TargetRules
{
	public TopDownRPGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "TopDownRPG" } );
	}
}
