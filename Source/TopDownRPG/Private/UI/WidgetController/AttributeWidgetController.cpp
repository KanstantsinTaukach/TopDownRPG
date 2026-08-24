// Copyright K.Taukach


#include "UI/WidgetController/AttributeWidgetController.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "Player/TDRPGPlayerState.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeWidgetController::BroadcastInitialValues()
{
    check(AttributeInfo);

    for(auto& Pair: GetTDRPGAttributeSet()->TagsToAttributes)
    {
        BroadcastAttributeInfo(Pair.Key, Pair.Value());
    }

    OnPlayerAttributePointsChangedDelegate.Broadcast(GetTDRPGPlayerState()->GetAttributePoints());
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{
    check(AttributeInfo);
    
    for(auto& Pair: GetTDRPGAttributeSet()->TagsToAttributes)
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
            [this, Pair](const FOnAttributeChangeData& Data)
        {
            BroadcastAttributeInfo(Pair.Key, Pair.Value());
        });
    }

    GetTDRPGPlayerState()->OnAttributePointsChangedDelegate.AddLambda([this](int32 AttributePoints)
    {
       OnPlayerAttributePointsChangedDelegate.Broadcast(AttributePoints);
    });
    GetTDRPGPlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
    {
        OnPlayerSpellPointsChangedDelegate.Broadcast(SpellPoints);
    });
}

void UAttributeWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{
    FTDRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
    Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
    AttributeInfoDelegate.Broadcast(Info);

    OnPlayerAttributePointsChangedDelegate.Broadcast(GetTDRPGPlayerState()->GetAttributePoints());
}

void UAttributeWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
    if(GetTDRPGAbilitySystemComponent())
    {
        GetTDRPGAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
    }
}