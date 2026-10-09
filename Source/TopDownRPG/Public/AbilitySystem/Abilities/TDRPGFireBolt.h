// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TDRPGProjectileSpell.h"
#include "TDRPGFireBolt.generated.h"

UCLASS()
class TOPDOWNRPG_API UTDRPGFireBolt : public UTDRPGProjectileSpell
{
	GENERATED_BODY()

public:
    virtual FString GetDescription (int32 Level) override;
    virtual FString GetNextLevelDescription (int32 Level) override;
};
