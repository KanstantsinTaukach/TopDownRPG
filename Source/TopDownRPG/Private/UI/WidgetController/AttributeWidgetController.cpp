// Copyright K.Taukach


#include "UI/WidgetController/AttributeWidgetController.h"
#include "Player/TDRPGPlayerState.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeWidgetController::BroadcastInitialValues()
{
    UTDRPGAttributeSet* AS = CastChecked<UTDRPGAttributeSet>(AttributeSet);
    check(AttributeInfo);

    for(auto& Pair: AS->TagsToAttributes)
    {
        BroadcastAttributeInfo(Pair.Key, Pair.Value());
    }
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{
    UTDRPGAttributeSet* AS = CastChecked<UTDRPGAttributeSet>(AttributeSet);
    check(AttributeInfo);
    
    for(auto& Pair: AS->TagsToAttributes)
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
            [this, Pair](const FOnAttributeChangeData& Data)
        {
            BroadcastAttributeInfo(Pair.Key, Pair.Value());
        });
    }

    ATDRPGPlayerState* TDRPGPlayerState = CastChecked<ATDRPGPlayerState>(PlayerState);
    TDRPGPlayerState->OnAttributePointsChangedDelegate.AddLambda([this](int32 AttributePoints)
    {
       OnPlayerAttributePointsChangedDelegate.Broadcast(AttributePoints);
    });
    TDRPGPlayerState->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
    {
        OnPlayerSpellPointsChangedDelegate.Broadcast(SpellPoints);
    });
}

void UAttributeWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
    FTDRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
    Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
    AttributeInfoDelegate.Broadcast(Info);

    ATDRPGPlayerState* TDRPGPlayerState = CastChecked<ATDRPGPlayerState>(PlayerState);
    OnPlayerAttributePointsChangedDelegate.Broadcast(TDRPGPlayerState->GetAttributePoints());
}