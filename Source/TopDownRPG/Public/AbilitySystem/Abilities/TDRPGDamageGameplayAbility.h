// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "TDRPGCombatInterface.h"
#include "AbilitySystem/Abilities/TDRPGGameplayAbility.h"
#include "TDRPGDamageGameplayAbility.generated.h"

UCLASS()
class TOPDOWNRPG_API UTDRPGDamageGameplayAbility : public UTDRPGGameplayAbility
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void CauseDamage(AActor* TargetActor);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FGameplayTag DamageType;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FScalableFloat Damage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DebuffChance = 20.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DebuffDamage = 5.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DebuffFrequency = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DebuffDuration = 5.0f;    

    UFUNCTION(BlueprintPure)
    FTaggedMontage GetRandomTaggedMontageFromAttay(const TArray<FTaggedMontage>& TaggedMontages) const;
};
