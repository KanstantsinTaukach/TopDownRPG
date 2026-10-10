// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/TDRPGWidgetController.h"
#include "GameplayTagContainer.h"
#include "TDRPGGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

struct FSelectedAbility
{
    FGameplayTag Ability = FGameplayTag();
    FGameplayTag Status = FGameplayTag();
};

UCLASS(BlueprintType, Blueprintable)
class TOPDOWNRPG_API USpellMenuWidgetController : public UTDRPGWidgetController
{
	GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;

    UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenu")
    FOnPlayerStatChangedSignature OnPlayerSpellPointsChangedDelegate;
    
    UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenu")
    FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

    UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenu")
    FWaitForEquipSelectionSignature WaitForEquipDelegate;
    UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenu")
    FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;
    
    UFUNCTION(BlueprintCallable)
    void SpellGlobeSelected(const FGameplayTag& AbilityTag);
    UFUNCTION(BlueprintCallable)
    void SpellGlobeDeselected();

    UFUNCTION(BlueprintCallable)
    void SpendPointButtonPressed();
    UFUNCTION(BlueprintCallable)
    void EquipButtonPressed();

private:
    static void ShouldEnableButtons(const FGameplayTag& AbilityStatusTag, int32 SpellPoints, bool&  bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton);

    FSelectedAbility SelectedAbility = {FTDRPGGameplayTags::Get().Abilities_None, FTDRPGGameplayTags::Get().Abilities_Status_Locked};
    int32 CurrentSpellPoints = 0;
    bool bWaitingForEquipSelection = false;
};
 