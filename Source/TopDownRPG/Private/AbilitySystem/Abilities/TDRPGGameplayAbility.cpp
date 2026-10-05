// Copyright K.Taukach

#include "AbilitySystem/Abilities/TDRPGGameplayAbility.h"

FString UTDRPGGameplayAbility::GetDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name", Level);
}

FString UTDRPGGameplayAbility::GetNextLevelDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n <Default>Causes much more damage </>"), Level);
}

FString UTDRPGGameplayAbility::GetLockedDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>Spell Locked Until Level: </><Level>%d</>"), Level);
}