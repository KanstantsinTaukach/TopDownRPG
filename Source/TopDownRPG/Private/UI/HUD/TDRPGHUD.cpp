// Copyright K.Taukach

#include "UI/HUD/TDRPGHUD.h"
#include "UI/Widget/TDRPGUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* ATDRPGHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
    if(OverlayWidgetController == nullptr)
    {
        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
        OverlayWidgetController->BindCallbacksToDependencies();
    }
    return OverlayWidgetController;
}

UAttributeWidgetController* ATDRPGHUD::GetAttributeWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
    if(AttributeWidgetController == nullptr)
    {
        AttributeWidgetController = NewObject<UAttributeWidgetController>(this, AttributeWidgetControllerClass);
        AttributeWidgetController->SetWidgetControllerParams(WidgetControllerParams);
        AttributeWidgetController->BindCallbacksToDependencies();
    }
    return AttributeWidgetController;
}

USpellMenuWidgetController* ATDRPGHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
    if(SpellMenuWidgetController == nullptr)
    {
        SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
        SpellMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
        SpellMenuWidgetController->BindCallbacksToDependencies();
    }
    return SpellMenuWidgetController;
}

void ATDRPGHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_TDRPGHUD"));
    checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_TDRPGHUD"));
    
    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget = Cast<UTDRPGUserWidget>(Widget);

    const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
    UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

    OverlayWidget->SetWidgetController(WidgetController);
    WidgetController->BroadcastInitialValues();
    Widget->AddToViewport();
}
