// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDRPGHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UTDRPGUserWidget;

struct FWidgetControllerParams;

UCLASS()
class TOPDOWNRPG_API ATDRPGHUD : public AHUD
{
	GENERATED_BODY()

public:
    UPROPERTY()
    TObjectPtr<UTDRPGUserWidget> OverlayWidget;

    UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);

    void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UTDRPGUserWidget> OverlayWidgetClass;

    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
