// Copyright K.Taukach

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TDRPGAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    const UTDRPGAttributeSet* TDRPGAttributeSet = CastChecked<UTDRPGAttributeSet>(AttributeSet);
    
    OnHealthChanged.Broadcast(TDRPGAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(TDRPGAttributeSet->GetMaxHealth());
}