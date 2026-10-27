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

FGameplayTag UTDRPGAbilitySystemComponent::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
    if(const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
    {
        return GetStatusTagFromSpec(*Spec);
    }
    return FGameplayTag();
}

FGameplayTag UTDRPGAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
    if(const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
    {
        return GetInputTagFromSpec(*Spec);
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
    for(const FTDRPGAbilityInfo& Info : AbilityInfo->AbilityInformation)
    {
        if(!Info.AbilityTag.IsValid()) continue;
        
        if(Info.LevelRequirement <= Level)
        {
            if(GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
            {
                int32 StartAbilityLevel = 1;
                FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, StartAbilityLevel);
                AbilitySpec.GetDynamicSpecSourceTags().AddTag(FTDRPGGameplayTags::Get().Abilities_Status_Eligible);
                GiveAbility(AbilitySpec);

                MarkAbilitySpecDirty(AbilitySpec);
                ClientUpdateAbilityStatus(Info.AbilityTag, FTDRPGGameplayTags::Get().Abilities_Status_Eligible, StartAbilityLevel);
            }
        }
    }
}

void UTDRPGAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
    AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
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

void UTDRPGAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
    if(FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
    {
        if(GetAvatarActor()->Implements<UTDRPGPlayerInterface>())
        {
            ITDRPGPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
        }
        
        const FTDRPGGameplayTags GameplayTags = FTDRPGGameplayTags::Get();

        FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);
        if(StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
        {
            AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Eligible);
            AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Unlocked);
            StatusTag = GameplayTags.Abilities_Status_Unlocked;
        }
        else if(StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
        {
            AbilitySpec->Level += 1;            
        }

        ClientUpdateAbilityStatus(AbilityTag, StatusTag, AbilitySpec->Level);
        MarkAbilitySpecDirty(*AbilitySpec);
    }
}

void UTDRPGAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Slot)
{
    if(FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
    {
        const FTDRPGGameplayTags GameplayTags = FTDRPGGameplayTags::Get();
        const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
        const FGameplayTag& Status = GetStatusTagFromSpec(*AbilitySpec);

        const bool bStatusValid = Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_Unlocked;
        if(bStatusValid)
        {
            // Remove this InputTag (slot) from any Ability that has it.
            ClearAbilitiesOfSlot(Slot);
            // Clear this ability's slot, just in case, it's a different slot
            ClearSlot(AbilitySpec);
            // Now, assign this ability to this slot
            AbilitySpec->GetDynamicSpecSourceTags().AddTag(Slot);
            if(Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
            {
                AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Unlocked);
                AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Equipped);
            }
            MarkAbilitySpecDirty(*AbilitySpec);
        }

        ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
    }
}

void UTDRPGAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
    AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

bool UTDRPGAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
    if(const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
    {
        if(UTDRPGGameplayAbility* TDRPGAbility = Cast<UTDRPGGameplayAbility>(AbilitySpec->Ability))
        {
            OutDescription = TDRPGAbility->GetDescription(AbilitySpec->Level);
            OutNextLevelDescription = TDRPGAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
            return true;
        }
    }
    
    const UAbilityInfo* AbilityInfo = UTDRPGAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
    if(!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FTDRPGGameplayTags::Get().Abilities_None))
    {
        OutDescription = FString();
    }
    else
    {
        OutDescription = UTDRPGGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
    }    
    OutNextLevelDescription = FString();    
    return false;
}

void UTDRPGAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
    const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
    Spec->GetDynamicSpecSourceTags().RemoveTag(Slot);
    MarkAbilitySpecDirty(*Spec);
}

void UTDRPGAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
    FScopedAbilityListLock ActiveScopeLock(*this);
    for(FGameplayAbilitySpec& Spec : GetActivatableAbilities())
    {
        if(AbilityHasSlot(&Spec, Slot))
        {
            ClearSlot(&Spec);
        }
    }
}

bool UTDRPGAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
    for(FGameplayTag Tag : Spec->GetDynamicSpecSourceTags())
    {
        if(Tag.MatchesTagExact(Slot))
        {
            return true;
        }
    }
    return false;
}