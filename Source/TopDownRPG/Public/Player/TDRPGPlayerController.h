// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDRPGPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class TOPDOWNRPG_API ATDRPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    ATDRPGPlayerController();
    
protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputMappingContext> PlayerMappingContext;
};
