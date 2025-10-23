// Copyright K.Taukach

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    const UTDRPGAttributeSet* TDRPGAttributeSet = CastChecked<UTDRPGAttributeSet>(AttributeSet);
    
    OnHealthChanged.Broadcast(TDRPGAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(TDRPGAttributeSet->GetMaxHealth());

    OnManaChanged.Broadcast(TDRPGAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(TDRPGAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    const UTDRPGAttributeSet* TDRPGAttributeSet = CastChecked<UTDRPGAttributeSet>(AttributeSet);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnHealthChanged.Broadcast(Data.NewValue);
    });

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetMaxHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnMaxHealthChanged.Broadcast(Data.NewValue);
    });

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnManaChanged.Broadcast(Data.NewValue);
    });
    
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetMaxManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnMaxManaChanged.Broadcast(Data.NewValue);
    });
        
    Cast<UTDRPGAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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