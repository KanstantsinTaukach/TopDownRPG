// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TDRPGAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGivenSignature, UTDRPGAbilitySystemComponent*);

UCLASS()
class TOPDOWNRPG_API UTDRPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    void AbilityActorInfoSet();

    FEffectAssetTagsSignature EffectAssetTags;
    FAbilitiesGivenSignature AbilitiesGiven;

    void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
    bool IsStartupAbilitiesGiven() const { return bStartupAbilitiesGiven; };

    void AbilityInputTagHeld(const FGameplayTag& InputTag);
    void AbilityInputTagReleased(const FGameplayTag& InputTag);
    
protected:
    UFUNCTION(Client, Reliable)
    void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

private:
    bool bStartupAbilitiesGiven = false;
};
