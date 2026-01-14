// Copyright K.Taukach

#include "AbilitySystem/TDRPGAbilitySystemGlobals.h"

#include "TDRPGAbilityTypes.h"

FGameplayEffectContext* UTDRPGAbilitySystemGlobals::AllocGameplayEffectContext() const
{
    return new FTDRPGGameplayEffectContext();
}