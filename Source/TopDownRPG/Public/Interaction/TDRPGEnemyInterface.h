// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TDRPGEnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTDRPGEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class TOPDOWNRPG_API ITDRPGEnemyInterface
{
	GENERATED_BODY()

public:
    virtual void HighlightActor() = 0;
    virtual void UnHighlightActor() = 0;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void SetCombatTarget(AActor* InCombatTarget);
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    AActor* GetCombatTarget() const;
};
