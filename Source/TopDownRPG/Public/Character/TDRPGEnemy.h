// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Character/TDRPGCharacterBase.h"
#include "Interaction/TDRPGEnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "TDRPGEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class TOPDOWNRPG_API ATDRPGEnemy : public ATDRPGCharacterBase, public ITDRPGEnemyInterface
{
	GENERATED_BODY()

public:
    ATDRPGEnemy();

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnHealthChanged;
    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnMaxHealthChanged;

    /** Enemy Interface */
    virtual void HighlightActor() override;
    virtual void UnHighlightActor() override;
    /** end Enemy Interface */

    /** Combat Interface */
    virtual int32 GetPlayerLevel() const override { return Level; };
    /** end Combat Interface */
    
protected:
    virtual void BeginPlay() override;

    virtual void InitAbilityActorInfo() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterClassDefaults")
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UWidgetComponent> HealthBar;
};
