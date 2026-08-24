// Copyright K.Taukach

#include "UI/WidgetController/TDRPGWidgetController.h"
#include "Player/TDRPGPlayerController.h"
#include "Player/TDRPGPlayerState.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UTDRPGWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
    PlayerController = WidgetControllerParams.PlayerController;
    PlayerState = WidgetControllerParams.PlayerState;
    AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
    AttributeSet = WidgetControllerParams.AttributeSet;
}

void UTDRPGWidgetController::BroadcastInitialValues()
{    
}

void UTDRPGWidgetController::BindCallbacksToDependencies()
{
}

void UTDRPGWidgetController::BroadcastAbilityInfo()
{
    if(!GetTDRPGAbilitySystemComponent()->IsStartupAbilitiesGiven()) return;

    FForEachAbility BroadcastDelegate;
    BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
    {
        FTDRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(TDRPGAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
        Info.InputTag = TDRPGAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
        AbilityInfoDelegate.Broadcast(Info);
    });
    GetTDRPGAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}

ATDRPGPlayerController* UTDRPGWidgetController::GetTDRPGPlayerController()
{
    if(TDRPGPlayerController == nullptr)
    {
        TDRPGPlayerController = Cast<ATDRPGPlayerController>(PlayerController);
    }
    return TDRPGPlayerController;
}

ATDRPGPlayerState* UTDRPGWidgetController::GetTDRPGPlayerState()
{
    if(TDRPGPlayerState == nullptr)
    {
        TDRPGPlayerState = Cast<ATDRPGPlayerState>(PlayerState);
    }
    return TDRPGPlayerState;
}

UTDRPGAbilitySystemComponent* UTDRPGWidgetController::GetTDRPGAbilitySystemComponent()
{
    if(TDRPGAbilitySystemComponent == nullptr)
    {
        TDRPGAbilitySystemComponent = Cast<UTDRPGAbilitySystemComponent>(AbilitySystemComponent);
    }
    return TDRPGAbilitySystemComponent;
}

UTDRPGAttributeSet* UTDRPGWidgetController::GetTDRPGAttributeSet()
{
    if(TDRPGAttributeSet == nullptr)
    {
        TDRPGAttributeSet = Cast<UTDRPGAttributeSet>(AttributeSet);
    }
    return TDRPGAttributeSet;
}