// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDRPGPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ITDRPGEnemyInterface;

struct FInputActionValue;

UCLASS()
class TOPDOWNRPG_API ATDRPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    ATDRPGPlayerController();
    virtual void PlayerTick(float DeltaTime) override;
    
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputMappingContext> PlayerMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    void Move(const FInputActionValue& InputActionValue);

    void CursorTrace();

    TObjectPtr<ITDRPGEnemyInterface> LastActor;
    TObjectPtr<ITDRPGEnemyInterface> ThisActor;
};
