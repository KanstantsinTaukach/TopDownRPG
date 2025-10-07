// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDRPGHUD.generated.h"

class UTDRPGUserWidget;

UCLASS()
class TOPDOWNRPG_API ATDRPGHUD : public AHUD
{
	GENERATED_BODY()

public:
    UPROPERTY()
    TObjectPtr<UTDRPGUserWidget> OverlayWidget;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UTDRPGUserWidget> OverlayWidgetClass;
};
