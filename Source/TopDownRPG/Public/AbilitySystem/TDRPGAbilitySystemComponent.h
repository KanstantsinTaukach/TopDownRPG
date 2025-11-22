// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TDRPGAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /*AssetTags*/);

UCLASS()
class TOPDOWNRPG_API UTDRPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    void AbilityActorInfoSet();

    FEffectAssetTagsSignature EffectAssetTags;

    void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

    void AbilityInputTagHeld(const FGameplayTag& InputTag);
    void AbilityInputTagReleased(const FGameplayTag& InputTag);
    
protected:
    void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
