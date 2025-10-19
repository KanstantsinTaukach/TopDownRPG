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
    
protected:
    void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
