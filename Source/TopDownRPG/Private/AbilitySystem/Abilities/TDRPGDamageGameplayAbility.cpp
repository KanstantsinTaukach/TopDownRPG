// Copyright K.Taukach

#include "AbilitySystem/Abilities/TDRPGDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UTDRPGDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
    FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
    const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
    
    GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));    
}

FTaggedMontage UTDRPGDamageGameplayAbility::GetRandomTaggedMontageFromAttay(const TArray<FTaggedMontage>& TaggedMontages) const
{
    if(TaggedMontages.Num() > 0)
    {
        const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
        return TaggedMontages[Selection];
    }
    return FTaggedMontage();
}