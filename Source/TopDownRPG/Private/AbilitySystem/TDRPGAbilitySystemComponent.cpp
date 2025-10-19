// Copyright K.Taukach


#include "AbilitySystem/TDRPGAbilitySystemComponent.h"

void UTDRPGAbilitySystemComponent::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDRPGAbilitySystemComponent::EffectApplied);
}

void UTDRPGAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
    FActiveGameplayEffectHandle ActiveEffectHandle)
{
    GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, FString("Effect Applied for AbilitySystemComponent"));
}