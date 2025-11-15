// Copyright K.Taukach


#include "UI/WidgetController/AttributeWidgetController.h"
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
}

void UAttributeWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
    FTDRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
    Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
    AttributeInfoDelegate.Broadcast(Info);
}