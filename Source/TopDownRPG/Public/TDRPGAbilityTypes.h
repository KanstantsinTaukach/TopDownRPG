#pragma once

#include "GameplayEffectTypes.h"
#include "TDRPGAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FTDRPGGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

public:
    /** Returns the actual struct used for serialization, subclasses must override this! */
    virtual UScriptStruct* GetScriptStruct() const override
    {
        return FGameplayEffectContext::GetScriptStruct();
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