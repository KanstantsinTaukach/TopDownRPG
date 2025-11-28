// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TDRPGGameplayAbility.h"
#include "TDRPGProjectileSpell.generated.h"

class ATDRPGProjectile;

UCLASS()
class TOPDOWNRPG_API UTDRPGProjectileSpell : public UTDRPGGameplayAbility
{
	GENERATED_BODY()

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<ATDRPGProjectile> ProjectileClass;
};
