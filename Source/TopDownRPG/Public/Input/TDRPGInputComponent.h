// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "TDRPGInputConfig.h"
#include "TDRPGInputComponent.generated.h"

UCLASS()
class TOPDOWNRPG_API UTDRPGInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
    template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
    void BindAbilityActions(const UTDRPGInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UTDRPGInputComponent::BindAbilityActions(const UTDRPGInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
    ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
    check(InputConfig);

    for(const FTDRPGInputAction& Action : InputConfig->AbilityInputActions)
    {
        if(PressedFunc)
        {
            BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
        }

        if(ReleasedFunc)
        {
            BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
        }

        if(HeldFunc)
        {
            BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
        }
    }
}
