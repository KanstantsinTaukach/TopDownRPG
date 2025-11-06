// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TDRPGAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;

UCLASS()
class TOPDOWNRPG_API UTDRPGAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "TDRPGAbilitySystemLibrary|WidgetController")
    static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
};
