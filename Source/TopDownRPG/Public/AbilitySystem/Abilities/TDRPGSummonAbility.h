// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TDRPGGameplayAbility.h"
#include "TDRPGSummonAbility.generated.h"

UCLASS()
class TOPDOWNRPG_API UTDRPGSummonAbility : public UTDRPGGameplayAbility
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    TArray<FVector> GetSpawnLocations();

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    int32 NumMinions = 5;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    TArray<TSubclassOf<APawn>> MinionClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    float MinSpawnDistance = 75.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    float MaxSpawnDistance = 300.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    float SpawnSpread = 90.0f;
};
