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
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

    Cast<UTDRPGAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
        [](const FGameplayTagContainer& AssetTags)
        {
            for(const FGameplayTag& Tag : AssetTags)
            {
                const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Msg);
            }
        }
    );
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
    OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
    OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
    OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
    OnMaxManaChanged.Broadcast(Data.NewValue);
}