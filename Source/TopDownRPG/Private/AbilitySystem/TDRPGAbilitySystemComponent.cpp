// Copyright K.Taukach


#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "TDRPGGameplayTags.h"

void UTDRPGAbilitySystemComponent::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDRPGAbilitySystemComponent::EffectApplied);
}

void UTDRPGAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
    for(TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        //GiveAbility(AbilitySpec);
        GiveAbilityAndActivateOnce(AbilitySpec);
    }
}

void UTDRPGAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
    FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);

    EffectAssetTags.Broadcast(TagContainer);
}