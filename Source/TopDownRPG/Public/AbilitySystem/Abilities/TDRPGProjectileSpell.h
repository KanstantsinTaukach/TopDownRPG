// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TDRPGGameplayAbility.h"
#include "TDRPGProjectileSpell.generated.h"

UCLASS()
class TOPDOWNRPG_API UTDRPGProjectileSpell : public UTDRPGGameplayAbility
{
	GENERATED_BODY()

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
