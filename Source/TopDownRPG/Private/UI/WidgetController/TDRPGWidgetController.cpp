// Copyright K.Taukach

#include "UI/WidgetController/TDRPGWidgetController.h"

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