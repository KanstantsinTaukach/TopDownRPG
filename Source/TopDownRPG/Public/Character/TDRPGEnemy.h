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

    /** Enemy Interface */
    virtual void HighlightActor() override;
    virtual void UnHighlightActor() override;
    /** end Enemy Interface */
    
protected:
    virtual void BeginPlay() override;
};
