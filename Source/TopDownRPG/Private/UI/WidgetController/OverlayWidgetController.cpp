// Copyright K.Taukach

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TDRPGAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    const UTDRPGAttributeSet* TDRPGAttributeSet = CastChecked<UTDRPGAttributeSet>(AttributeSet);
    
    OnHealthChanged.Broadcast(TDRPGAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(TDRPGAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    const UTDRPGAttributeSet* TDRPGAttributeSet = CastChecked<UTDRPGAttributeSet>(AttributeSet);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
    OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
    OnMaxHealthChanged.Broadcast(Data.NewValue);
}