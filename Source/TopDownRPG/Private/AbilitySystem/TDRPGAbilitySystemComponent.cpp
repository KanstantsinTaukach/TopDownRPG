// Copyright K.Taukach

#include "AbilitySystem/TDRPGAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Abilities/TDRPGGameplayAbility.h"
#include "TopDownRPG/TDRPGLogChannels.h"
#include "TDRPGGameplayTags.h"
#include "Interaction/TDRPGPlayerInterface.h"

void UTDRPGAbilitySystemComponent::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDRPGAbilitySystemComponent::ClientEffectApplied);
}

void UTDRPGAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
    for(const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        if(const UTDRPGGameplayAbility* TDRPGAbility = Cast<UTDRPGGameplayAbility>(AbilitySpec.Ability))
        {
            //AbilitySpec.DynamicAbilityTags.AddTag((TDRPGAbility->StartupInputTag));
            AbilitySpec.GetDynamicSpecSourceTags().AddTag((TDRPGAbility->StartupInputTag));
            GiveAbility(AbilitySpec);
        }
    }

    bStartupAbilitiesGiven = true;
    AbilitiesGiven.Broadcast(this);
}

void UTDRPGAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
    for(const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        GiveAbilityAndActivateOnce(AbilitySpec);
    }
}

void UTDRPGAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    if(!InputTag.IsValid()) return;

    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if(AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec);
            if(!AbilitySpec.IsActive())
            {
                TryActivateAbility(AbilitySpec.Handle);
            }
        }
    }
}

void UTDRPGAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if(!InputTag.IsValid()) return;

    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if(AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
        {
            AbilitySpecInputReleased(AbilitySpec);
        }
    }
}

void UTDRPGAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
    FScopedAbilityListLock ActiveScopeLock(*this);
    for(const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if(!Delegate.ExecuteIfBound(AbilitySpec))
        {
            UE_LOG(LogTDRPG, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
        }
    }
}

FGameplayTag UTDRPGAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability)
    {
        for(FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Abilities")))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UTDRPGAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for(FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
        {
            return Tag;
        }
    }
    return FGameplayTag();
}

void UTDRPGAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
    FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);

    EffectAssetTags.Broadcast(TagContainer);
}

void UTDRPGAbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities();

    if(!bStartupAbilitiesGiven)
    {
        bStartupAbilitiesGiven = true;
        AbilitiesGiven.Broadcast(this);
    }    
}

void UTDRPGAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
    if(GetAvatarActor()->Implements<UTDRPGPlayerInterface>())
    {
        if(ITDRPGPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
        {
            ServerUpgradeAttribute_Implementation(AttributeTag);
        }
    }
}

void UTDRPGAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
    FGameplayEventData Payload;
    Payload.EventTag = AttributeTag;
    Payload.EventMagnitude = 1.0f;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

    if(GetAvatarActor()->Implements<UTDRPGPlayerInterface>())
    {
        ITDRPGPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
    }
}