// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Character/TDRPGCharacterBase.h"
#include "Interaction/TDRPGEnemyInterface.h"
#include "TDRPGEnemy.generated.h"

UCLASS()
class TOPDOWNRPG_API ATDRPGEnemy : public ATDRPGCharacterBase, public ITDRPGEnemyInterface
{
	GENERATED_BODY()

public:
    ATDRPGEnemy();
    virtual void HighlightActor() override;
    virtual void UnHighlightActor() override;
};
