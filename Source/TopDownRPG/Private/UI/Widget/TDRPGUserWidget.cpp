// Copyright K.Taukach

#include "UI/Widget/TDRPGUserWidget.h"

void UTDRPGUserWidget::SetWidgetController(UObject* InWidgetController)
{
    WidgetController = InWidgetController;
    WidgetControllerSet();
}