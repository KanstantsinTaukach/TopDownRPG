// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TDRPGAbilitySystemComponent.generated.h"

UCLASS()
class TOPDOWNRPG_API UTDRPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    void AbilityActorInfoSet();
    
protected:
    void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
