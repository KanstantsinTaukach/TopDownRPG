// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TDRPGCombatInterface.generated.h"

class UAnimMontage;

UINTERFACE(MinimalAPI, BlueprintType)
class UTDRPGCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class TOPDOWNRPG_API ITDRPGCombatInterface
{
	GENERATED_BODY()

public:
    virtual int32 GetPlayerLevel() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    FVector GetCombatSocketLocation();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void UpdateFacingTarget(const FVector& TargetLocation);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UAnimMontage* GetHitReactMontage();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UAnimMontage* GetAttackMontage();

    virtual void Die() = 0;
};
