// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDRPGHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UTDRPGUserWidget;
class UOverlayWidgetController;
class UAttributeWidgetController;
class USpellMenuWidgetController;

struct FWidgetControllerParams;

UCLASS()
class TOPDOWNRPG_API ATDRPGHUD : public AHUD
{
	GENERATED_BODY()

public:
    UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
    UAttributeWidgetController* GetAttributeWidgetController(const FWidgetControllerParams& WidgetControllerParams);
    USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);
    
    void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
    UPROPERTY()
    TObjectPtr<UTDRPGUserWidget> OverlayWidget;    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UTDRPGUserWidget> OverlayWidgetClass;
    
    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> OverlayWidgetController;    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
    
    UPROPERTY()
    TObjectPtr<UAttributeWidgetController> AttributeWidgetController;    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UAttributeWidgetController> AttributeWidgetControllerClass;

    UPROPERTY()
    TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;    
    UPROPERTY(EditAnywhere)
    TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
};
