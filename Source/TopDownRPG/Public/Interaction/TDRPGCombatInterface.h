// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TDRPGCombatInterface.generated.h"

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

     virtual FVector GetCombatSocketLocation();

     UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
     void UpdateFacingTarget(const FVector& TargetLocation);
};
