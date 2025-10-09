// Copyright K.Taukach

#include "AbilitySystem/TDRPGAttributeSet.h"
#include "Net/UnrealNetwork.h"

UTDRPGAttributeSet::UTDRPGAttributeSet()
{
    InitHealth(50.0f);
    InitMaxHealth(100.0f);
    InitMana(30.0f);
    InitMaxMana(100.0f);
}

void UTDRPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UTDRPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, Health, OldHealth);
}

void UTDRPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, MaxHealth, OldMaxHealth);
}

void UTDRPGAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, Mana, OldMana);
}

void UTDRPGAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, MaxMana, OldMaxMana);
}
