// Copyright K.Taukach

using UnrealBuildTool;
using System.Collections.Generic;

public class TopDownRPGTarget : TargetRules
{
	public TopDownRPGTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "TopDownRPG" } );
	}
}
