// Copyright K.Taukach

#include "UI/HUD/TDRPGHUD.h"
#include "UI/Widget/TDRPGUserWidget.h"

void ATDRPGHUD::BeginPlay()
{
    Super::BeginPlay();

    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
    Widget->AddToViewport();
}