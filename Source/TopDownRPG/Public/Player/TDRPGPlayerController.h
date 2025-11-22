// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "TDRPGPlayerController.generated.h"

class UTDRPGInputConfig;
class UInputMappingContext;
class UInputAction;
class ITDRPGEnemyInterface;
class UTDRPGAbilitySystemComponent;

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

    ITDRPGEnemyInterface* LastActor;
    ITDRPGEnemyInterface* ThisActor;

    void AbilityInputTagPressed(FGameplayTag InputTag);
    void AbilityInputTagReleased(FGameplayTag InputTag);
    void AbilityInputTagHeld(FGameplayTag InputTag);

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UTDRPGInputConfig> InputConfig;

    UPROPERTY()
    TObjectPtr<UTDRPGAbilitySystemComponent> AbilitySystemComponent;

    UTDRPGAbilitySystemComponent* GetASC();
};
