// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/TDRPGWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class TOPDOWNRPG_API USpellMenuWidgetController : public UTDRPGWidgetController
{
	GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;
	
};
