// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TDRPGWidgetContoller.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class TOPDOWNRPG_API UTDRPGWidgetContoller : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<APlayerState> PlayerState;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<UAttributeSet> AttributeSet;
};
