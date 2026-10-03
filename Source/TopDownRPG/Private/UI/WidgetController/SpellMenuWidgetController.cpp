// Copyright K.Taukach

#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "TDRPGGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "Player/TDRPGPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
    BroadcastAbilityInfo();

    OnPlayerSpellPointsChangedDelegate.Broadcast(GetTDRPGPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
    GetTDRPGAbilitySystemComponent()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
    {
        if(AbilityInfo)
        {
            FTDRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
            Info.StatusTag = StatusTag;
            AbilityInfoDelegate.Broadcast(Info);
        }
    });

    GetTDRPGPlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
    {
        OnPlayerSpellPointsChangedDelegate.Broadcast(SpellPoints);
    });
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
    const FTDRPGGameplayTags& Tags = FTDRPGGameplayTags::Get();
    
    UTDRPGAbilitySystemComponent* TDRPGASC = GetTDRPGAbilitySystemComponent();
    const FGameplayAbilitySpec* AbilitySpec = TDRPGASC ? TDRPGASC->GetSpecFromAbilityTag(AbilityTag) : nullptr;      
    
    FGameplayTag AbilityStatusTag = FGameplayTag();
        
    if(AbilityTag.MatchesTagExact(Tags.Abilities_None))
    {
        AbilityStatusTag = Tags.Abilities_Status_Locked;
    }
    else if(AbilitySpec)
    {
        AbilityStatusTag = TDRPGASC->GetStatusTagFromSpec(*AbilitySpec);
    }

    const ATDRPGPlayerState* PS = GetTDRPGPlayerState();
    const bool bHasSpellPoints = PS && PS->GetSpellPoints() > 0;
    
    bool bEnableSpellPoints = false;
    bool bEnableEquip = false;
    ShouldEnableButtons(AbilityStatusTag, bHasSpellPoints, bEnableSpellPoints, bEnableEquip);

    SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatusTag, bool bHasSpellPoints, bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton)
{
    const FTDRPGGameplayTags& Tags = FTDRPGGameplayTags::Get();

    bShouldEnableSpendPointsButton = false;
    bShouldEnableEquipButton = false;
    
    if(AbilityStatusTag.MatchesTagExact(Tags.Abilities_Status_Eligible))
    {
       bShouldEnableSpendPointsButton = bHasSpellPoints;
    }
    else if(AbilityStatusTag.MatchesTagExact(Tags.Abilities_Status_Unlocked) || AbilityStatusTag.MatchesTagExact(Tags.Abilities_Status_Equipped))
    {
        bShouldEnableEquipButton = true;        
        bShouldEnableSpendPointsButton = bHasSpellPoints;
    }
}