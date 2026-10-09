// Copyright K.Taukach

#include "AbilitySystem/Abilities/TDRPGGameplayAbility.h"
#include "AbilitySystem/TDRPGAttributeSet.h"

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

float UTDRPGGameplayAbility::GetManaCost(float InLevel) const
{
    float ManaCost = 0.0f;
    if(const UGameplayEffect* CostEffect = GetCostGameplayEffect())
    {
        for(FGameplayModifierInfo Mod : CostEffect->Modifiers)
        {
            if(Mod.Attribute == UTDRPGAttributeSet::GetManaAttribute())
            {
                Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
                break;
            }
        }
    }
    
    return ManaCost;
}

float UTDRPGGameplayAbility::GetCooldown(float InLevel) const
{
    float Cooldown = 0.0f;
    if(const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
    {
        CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
    }

    return Cooldown;
}