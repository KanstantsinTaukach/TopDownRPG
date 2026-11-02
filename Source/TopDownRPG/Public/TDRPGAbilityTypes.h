#pragma once

#include "GameplayEffectTypes.h"
#include "TDRPGAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
    GENERATED_BODY()

    FDamageEffectParams(){};

    UPROPERTY()
    TObjectPtr<UObject> WorldContextObject = nullptr;

    UPROPERTY()
    TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
    
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

    UPROPERTY()
    float BaseDamage = 0.0f;

    UPROPERTY()
    float AbilityLevel = 1.0f;

    UPROPERTY()
    FGameplayTag DamageType = FGameplayTag();

    UPROPERTY()
    float DebuffChance = 0.0f;

    UPROPERTY()
    float DebuffDamage = 0.0f;

    UPROPERTY()
    float DebuffDuration = 0.0f;

    UPROPERTY()
    float DebuffFrequency = 0.0f;
};

USTRUCT(BlueprintType)
struct FTDRPGGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

public:
    /** Returns the actual struct used for serialization, subclasses must override this! */
    virtual UScriptStruct* GetScriptStruct() const override
    {
        return FGameplayEffectContext::StaticStruct();
    }

    /** Creates a copy of this context, used to duplicate for later modifications */
    virtual FTDRPGGameplayEffectContext* Duplicate() const
    {
        FTDRPGGameplayEffectContext* NewContext = new FTDRPGGameplayEffectContext();
        *NewContext = *this;
        if (GetHitResult())
        {
            // Does a deep copy of the hit result
            NewContext->AddHitResult(*GetHitResult(), true);
        }
        return NewContext;
    }

    /** Custom serialization, subclasses must override this */
    virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

    bool IsCriticalHit() const { return bIsCriticalHit; }
    bool IsBlockedHit () const { return bIsBlockedHit; }

    void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
    void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
    
protected:
    UPROPERTY()
    bool bIsBlockedHit = false;

    UPROPERTY()
    bool bIsCriticalHit = false;
};

template<>
struct TStructOpsTypeTraits<FTDRPGGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FGameplayEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true
    };
};