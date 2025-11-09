// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/TDRPGWidgetController.h"
#include "AttributeWidgetController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class TOPDOWNRPG_API UAttributeWidgetController : public UTDRPGWidgetController
{
	GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;	
};
