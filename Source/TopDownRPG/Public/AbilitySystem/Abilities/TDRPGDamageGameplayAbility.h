// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
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
    TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
