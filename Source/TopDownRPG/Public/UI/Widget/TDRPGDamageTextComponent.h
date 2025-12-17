// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TDRPGDamageTextComponent.generated.h"

UCLASS()
class TOPDOWNRPG_API UTDRPGDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetDamageText(float Damage);
};
