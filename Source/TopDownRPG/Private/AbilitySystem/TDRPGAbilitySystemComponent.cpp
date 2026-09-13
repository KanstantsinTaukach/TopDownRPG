// Copyright K.Taukach

#include "AbilitySystem/TDRPGAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/TDRPGAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
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
            AbilitySpec.GetDynamicSpecSourceTags().AddTag((TDRPGAbility->StartupInputTag));
            AbilitySpec.GetDynamicSpecSourceTags().AddTag(FTDRPGGameplayTags::Get().Abilities_Status_Equipped);
            GiveAbility(AbilitySpec);
        }
    }

    bStartupAbilitiesGiven = true;
    AbilitiesGiven.Broadcast();
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

    for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
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

    for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
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
    if(AbilitySpec.Ability)
    {
        for(FGameplayTag AbilityTag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if(AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag("Abilities")))
            {
                return AbilityTag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UTDRPGAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for(FGameplayTag InputTag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if(InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
        {
            return InputTag;
        }
    }
    return FGameplayTag();
}

FGameplayTag UTDRPGAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for(FGameplayTag StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if(StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
        {
            return StatusTag;
        }
    }
    return FGameplayTag();
}

FGameplayAbilitySpec* UTDRPGAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
    FScopedAbilityListLock ActiveScopeLock(*this);
    for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        for(FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if(Tag.MatchesTag(AbilityTag))
            {
                return &AbilitySpec;
            }
        }
    }

    return nullptr;
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
        AbilitiesGiven.Broadcast();
    }    
}

void UTDRPGAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
    if(GetAvatarActor()->Implements<UTDRPGPlayerInterface>())
    {
        if(ITDRPGPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
        {
            ServerUpgradeAttribute(AttributeTag);
        }
    }
}

void UTDRPGAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
    UAbilityInfo* AbilityInfo = UTDRPGAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
    for (const FTDRPGAbilityInfo& Info : AbilityInfo->AbilityInformation)
    {
        if(!Info.AbilityTag.IsValid()) continue;
        
        if(Info.LevelRequirement <= Level)
        {
            if(GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
            {
                FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
                AbilitySpec.GetDynamicSpecSourceTags().AddTag(FTDRPGGameplayTags::Get().Abilities_Status_Eligible);
                GiveAbility(AbilitySpec);

                MarkAbilitySpecDirty(AbilitySpec);
            }
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