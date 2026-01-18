// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TDRPGDamageGameplayAbility.h"
#include "TDRPGProjectileSpell.generated.h"

class ATDRPGProjectile;

UCLASS()
class TOPDOWNRPG_API UTDRPGProjectileSpell : public UTDRPGDamageGameplayAbility
{
	GENERATED_BODY()

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void SpawnProjectile(const FVector& ProjectileTargetLocation);
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<ATDRPGProjectile> ProjectileClass;
};
