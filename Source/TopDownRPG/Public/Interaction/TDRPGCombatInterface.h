// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "TDRPGCombatInterface.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* Montage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag MontageTag;
};

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
    TArray<FTaggedMontage> GetAttackMontages();

    virtual void Die() = 0;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    bool IsDead() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    AActor* GetAvatar();
};
