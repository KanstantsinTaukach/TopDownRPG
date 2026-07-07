// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TDRPGPlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTDRPGPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class TOPDOWNRPG_API ITDRPGPlayerInterface
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintNativeEvent)
    void AddToXP(int32 InXP);

    UFUNCTION(BlueprintNativeEvent)
    void LevelUp();
};
