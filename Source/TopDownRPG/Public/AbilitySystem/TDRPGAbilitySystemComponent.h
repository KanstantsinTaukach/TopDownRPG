// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TDRPGAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenSignature);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);

UCLASS()
class TOPDOWNRPG_API UTDRPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    void AbilityActorInfoSet();

    FEffectAssetTagsSignature EffectAssetTags;
    FAbilitiesGivenSignature AbilitiesGiven;
    FAbilityStatusChanged AbilityStatusChanged;

    void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
    void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
    bool IsStartupAbilitiesGiven() const { return bStartupAbilitiesGiven; };

    void AbilityInputTagHeld(const FGameplayTag& InputTag);
    void AbilityInputTagReleased(const FGameplayTag& InputTag);
    
    void ForEachAbility(const FForEachAbility& Delegate);

    static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
    static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
    static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

    FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

    void UpgradeAttribute(const FGameplayTag& AttributeTag);

    void UpdateAbilityStatuses(int32 Level);

    UFUNCTION(Server, Reliable)
    void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
    
protected:
    UFUNCTION(Client, Reliable)
    void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

    UFUNCTION(Client, Reliable)
    void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);

    virtual void OnRep_ActivateAbilities() override;

    UFUNCTION(Server, Reliable)
    void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
    
private:
    bool bStartupAbilitiesGiven = false;
};
