// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/TDRPGWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled);

UCLASS(BlueprintType, Blueprintable)
class TOPDOWNRPG_API USpellMenuWidgetController : public UTDRPGWidgetController
{
	GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;

    UPROPERTY(BlueprintAssignable, Category = "GAS|SpellPoints")
    FOnPlayerStatChangedSignature OnPlayerSpellPointsChangedDelegate;
    UPROPERTY(BlueprintAssignable, Category = "GAS|SpellPoints")
    FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
    
    UFUNCTION(BlueprintCallable)
    void SpellGlobeSelected(const FGameplayTag& AbilityTag);

private:
    static void ShouldEnableButtons(const FGameplayTag& AbilityStatusTag, bool bHasSpellPoints, bool&  bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton);
};
 