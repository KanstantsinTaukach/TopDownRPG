// Copyright K.Taukach


#include "UI/WidgetController/AttributeWidgetController.h"

#include "TDRPGGameplayTags.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeWidgetController::BroadcastInitialValues()
{
    UTDRPGAttributeSet* AS = CastChecked<UTDRPGAttributeSet>(AttributeSet);

    check(AttributeInfo);

    for(auto& Pair: AS->TagsToAttributes)
    {
        FTDRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
        Info.AttributeValue = Pair.Value().GetNumericValue(AS);
        AttributeInfoDelegate.Broadcast(Info);
    }
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{

}