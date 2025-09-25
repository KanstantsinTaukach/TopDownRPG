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
};
