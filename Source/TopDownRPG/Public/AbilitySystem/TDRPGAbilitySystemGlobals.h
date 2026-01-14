// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "TDRPGAbilitySystemGlobals.generated.h"

UCLASS()
class TOPDOWNRPG_API UTDRPGAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
    virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
