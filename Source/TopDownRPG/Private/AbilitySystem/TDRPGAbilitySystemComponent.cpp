// Copyright K.Taukach


#include "AbilitySystem/TDRPGAbilitySystemComponent.h"

void UTDRPGAbilitySystemComponent::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDRPGAbilitySystemComponent::EffectApplied);
}

void UTDRPGAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
    FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);
    for(const FGameplayTag& Tag : TagContainer)
    {
        // TODO: Broadcast the tag to the Widget Controller

        const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Msg);
    }
}