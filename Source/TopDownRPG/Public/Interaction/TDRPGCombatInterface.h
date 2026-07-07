// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "TDRPGCombatInterface.generated.h"

class UAnimMontage;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> Montage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag MontageTag;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag SocketTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<USoundBase> ImpactSound = nullptr;
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
    UFUNCTION(BlueprintNativeEvent)
    int32 GetPlayerLevel() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    FVector GetCombatSocketLocation(const FGameplayTag& SocketTag) const;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void UpdateFacingTarget(const FVector& TargetLocation);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UAnimMontage* GetHitReactMontage() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    TArray<FTaggedMontage> GetAttackMontages() const;

    virtual void Die() = 0;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    bool IsDead() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    AActor* GetAvatar();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UNiagaraSystem* GetBloodEffect();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    int32 GetMinionCount() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void AddMinionCount(int32 Amount);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    ECharacterClass GetCharacterClass();
};
