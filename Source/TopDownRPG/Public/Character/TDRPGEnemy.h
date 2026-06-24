// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Character/TDRPGCharacterBase.h"
#include "Interaction/TDRPGEnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "TDRPGEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ATDRPGAIController;

UCLASS()
class TOPDOWNRPG_API ATDRPGEnemy : public ATDRPGCharacterBase, public ITDRPGEnemyInterface
{
	GENERATED_BODY()

public:
    ATDRPGEnemy();

    virtual void PossessedBy(AController* NewController) override;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnHealthChanged;
    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnMaxHealthChanged;

    /** Enemy Interface */
    virtual void HighlightActor() override;
    virtual void UnHighlightActor() override;

    virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
    virtual AActor* GetCombatTarget_Implementation() const override;
    /** end Enemy Interface */

    /** Combat Interface */
    virtual int32 GetPlayerLevel() const override { return Level; };

    virtual void Die() override;
    /** end Combat Interface */

    void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bHitReacting = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float BaseWalkSpeed = 250.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float LifeSpan = 5.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    TObjectPtr<AActor> CombatTarget;
    
protected:
    virtual void BeginPlay() override;

    virtual void InitAbilityActorInfo() override;

    virtual void InitializeDefaultAttributes() const override;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterClassDefaults")
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UWidgetComponent> HealthBar;

    UPROPERTY(EditAnywhere, Category="AI")
    TObjectPtr<UBehaviorTree> BehaviorTree;

    UPROPERTY()
    TObjectPtr<ATDRPGAIController> TDRPGAIController;
};
