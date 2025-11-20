// Copyright K.Taukach

#include "Input/TDRPGInputConfig.h"

const UInputAction* UTDRPGInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    for(const FTDRPGInputAction& Action : AbilityInputActions)
    {
        if(Action.InputTag == InputTag && Action.InputAction) return Action.InputAction;
    }

    if(bLogNotFound)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
    }
    
    return nullptr;
}