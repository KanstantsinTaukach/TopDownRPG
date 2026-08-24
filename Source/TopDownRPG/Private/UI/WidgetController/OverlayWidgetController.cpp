// Copyright K.Taukach

#include "UI/WidgetController/OverlayWidgetController.h"

#include "LevelUpInfo.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "Player/TDRPGPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    OnHealthChanged.Broadcast(GetTDRPGAttributeSet()->GetHealth());
    OnMaxHealthChanged.Broadcast(GetTDRPGAttributeSet()->GetMaxHealth());

    OnManaChanged.Broadcast(GetTDRPGAttributeSet()->GetMana());
    OnMaxManaChanged.Broadcast(GetTDRPGAttributeSet()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{    
    GetTDRPGPlayerState()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
    GetTDRPGPlayerState()->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel)
    {
        OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
    });
    
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTDRPGAttributeSet()->GetHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnHealthChanged.Broadcast(Data.NewValue);
    });

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTDRPGAttributeSet()->GetMaxHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnMaxHealthChanged.Broadcast(Data.NewValue);
    });

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTDRPGAttributeSet()->GetManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnManaChanged.Broadcast(Data.NewValue);
    });
    
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTDRPGAttributeSet()->GetMaxManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnMaxManaChanged.Broadcast(Data.NewValue);
    });

    if(GetTDRPGAbilitySystemComponent())
    {
        if(GetTDRPGAbilitySystemComponent()->IsStartupAbilitiesGiven())
        {
            BroadcastAbilityInfo();
        }
        else
        {
            GetTDRPGAbilitySystemComponent()->AbilitiesGiven.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
        }        
    
        GetTDRPGAbilitySystemComponent()->EffectAssetTags.AddLambda(
        [this](const FGameplayTagContainer& AssetTags)
        {
            for(const FGameplayTag& Tag : AssetTags)
            {
                // "Message.HealthPotion".MatchesTag("Message") will return true, "Message".MatchesTag("Message.HealthPotion") will return false

                FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
                if(Tag.MatchesTag(MessageTag))
                {
                    const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
                    if(Row != nullptr)
                    {
                        MessageWidgetRowDelegate.Broadcast(*Row);
                    }                    
                }
            }
        });
    }
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
    const ULevelUpInfo* LevelUpInfo = GetTDRPGPlayerState()->LevelUpInfo;
    checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out TDRTGPlayerState Blueprint."));

    const int32 PlayerLevel = LevelUpInfo->FindLevelForXP(NewXP);
    const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

    if(PlayerLevel <= MaxLevel && PlayerLevel > 0)
    {
        const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[PlayerLevel].LevelUpRequirement;
        const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[PlayerLevel - 1].LevelUpRequirement;
        
        const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
        const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

        const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
        OnXPPercentChanged.Broadcast(XPBarPercent);
    }
}