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
            GetTDRPGAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
            
            SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
        }
    });

    GetTDRPGAbilitySystemComponent()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

    GetTDRPGPlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
    {
        OnPlayerSpellPointsChangedDelegate.Broadcast(SpellPoints);
        
        CurrentSpellPoints = SpellPoints;
        bool bEnableSpellPoints = false;
        bool bEnableEquip = false;
        ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);
        
        FString Description;
        FString NextLevelDescription;
        GetTDRPGAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
            
        SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
    });
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
    if(bWaitingForEquipSelection)
    {
        const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
        StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
        bWaitingForEquipSelection = false;
    }
    
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
    GetTDRPGAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
            
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
    if(bWaitingForEquipSelection)
    {
        const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
        StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
        bWaitingForEquipSelection = false;
    }
    
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

void USpellMenuWidgetController::EquipButtonPressed()
{
    const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

    WaitForEquipDelegate.Broadcast(AbilityType);
    bWaitingForEquipSelection = true;

    const FGameplayTag SelectedStatus = GetTDRPGAbilitySystemComponent()->GetStatusTagFromAbilityTag(SelectedAbility.Ability);
    if(SelectedStatus.MatchesTagExact(FTDRPGGameplayTags::Get().Abilities_Status_Equipped))
    {
        SelectedSlot = GetTDRPGAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
    }
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
    if(!bWaitingForEquipSelection) return;

    // Check selected ability against the slot's ability type.
    // (don't equip an offensive spell in a passive slot and vice versa)
    const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
    if(!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

    GetTDRPGAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
    bWaitingForEquipSelection = false;

    const FTDRPGGameplayTags& GameplayTags = FTDRPGGameplayTags::Get();
    FTDRPGAbilityInfo LastSlotInfo;
    LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
    LastSlotInfo.InputTag = PreviousSlot;
    LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
    // Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
    AbilityInfoDelegate.Broadcast(LastSlotInfo);

    FTDRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
    Info.StatusTag = Status;
    Info.InputTag = Slot;
    AbilityInfoDelegate.Broadcast(Info);

    StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
    SpellGlobeReassignDelegate.Broadcast(AbilityTag);
    SpellGlobeDeselected();
}