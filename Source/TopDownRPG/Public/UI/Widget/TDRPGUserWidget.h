// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDRPGUserWidget.generated.h"

UCLASS()
class TOPDOWNRPG_API UTDRPGUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetWidgetController(UObject* InWidgetController);
    
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UObject> WidgetController;

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void WidgetControllerSet();
};
