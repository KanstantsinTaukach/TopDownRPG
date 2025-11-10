// Copyright K.Taukach


#include "UI/WidgetController/AttributeWidgetController.h"

#include "TDRPGGameplayTags.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeWidgetController::BroadcastInitialValues()
{
    UTDRPGAttributeSet* AS = CastChecked<UTDRPGAttributeSet>(AttributeSet);

    check(AttributeInfo);

    FTDRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FTDRPGGameplayTags::Get().Attributes_Primary_Strength);
    Info.AttributeValue = AS->GetStrength();
    AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{

}