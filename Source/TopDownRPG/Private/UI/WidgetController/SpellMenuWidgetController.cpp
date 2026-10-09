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
    GetTDRPGAbilitySystemComponent()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
    {
        if(AbilityInfo)
        {
            FTDRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
            Info.StatusTag = StatusTag;
            AbilityInfoDelegate.Broadcast(Info);
        }

        if(SelectedAbility.Ability.MatchesTagExact(AbilityTag))
        {
            SelectedAbility.Status = StatusTag;
            bool bEnableSpellPoints = false;
            bool bEnableEquip = false;
            ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);

            FString Description;
            FString NextLevelDescription;
            GetTDRPGAbilitySystemComponent()->GetDescriptonsByAbilityTag(AbilityTag, Description, NextLevelDescription);
            
            SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
        }
    });

    GetTDRPGPlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
    {
        OnPlayerSpellPointsChangedDelegate.Broadcast(SpellPoints);
        
        CurrentSpellPoints = SpellPoints;
        bool bEnableSpellPoints = false;
        bool bEnableEquip = false;
        ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);
        
        FString Description;
        FString NextLevelDescription;
        GetTDRPGAbilitySystemComponent()->GetDescriptonsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
            
        SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
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

    SelectedAbility.Ability = AbilityTag;
    SelectedAbility.Status = AbilityStatusTag;
    
    const ATDRPGPlayerState* PS = GetTDRPGPlayerState();
    if(!PS) return;
    const int32 SpellPoints = PS->GetSpellPoints();
    
    bool bEnableSpellPoints = false;
    bool bEnableEquip = false;
    ShouldEnableButtons(AbilityStatusTag, SpellPoints, bEnableSpellPoints, bEnableEquip);

    FString Description;
    FString NextLevelDescription;
    GetTDRPGAbilitySystemComponent()->GetDescriptonsByAbilityTag(AbilityTag, Description, NextLevelDescription);
            
    SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatusTag, int32 SpellPoints, bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton)
{
    const FTDRPGGameplayTags& Tags = FTDRPGGameplayTags::Get();

    bShouldEnableSpendPointsButton = false;
    bShouldEnableEquipButton = false;
    
    if(AbilityStatusTag.MatchesTagExact(Tags.Abilities_Status_Eligible))
    {
       bShouldEnableSpendPointsButton = SpellPoints > 0 ? true : false;
    }
    else if(AbilityStatusTag.MatchesTagExact(Tags.Abilities_Status_Unlocked) || AbilityStatusTag.MatchesTagExact(Tags.Abilities_Status_Equipped))
    {
        bShouldEnableEquipButton = true;        
        bShouldEnableSpendPointsButton = SpellPoints > 0 ? true : false;
    }
}

void USpellMenuWidgetController::SpellGlobeDeselected()
{
    const FTDRPGGameplayTags& Tags = FTDRPGGameplayTags::Get();
    SelectedAbility.Ability = Tags.Abilities_None;
    SelectedAbility.Status = Tags.Abilities_Status_Locked;

    SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
    UTDRPGAbilitySystemComponent* TDRPGASC = GetTDRPGAbilitySystemComponent();
    if(TDRPGASC)
    {
        TDRPGASC->ServerSpendSpellPoint(SelectedAbility.Ability);
    }
}