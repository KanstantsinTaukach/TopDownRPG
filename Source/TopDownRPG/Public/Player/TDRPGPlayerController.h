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
class USplineComponent;

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
    FHitResult CursorHit;

    void AbilityInputTagPressed(FGameplayTag InputTag);
    void AbilityInputTagReleased(FGameplayTag InputTag);
    void AbilityInputTagHeld(FGameplayTag InputTag);

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UTDRPGInputConfig> InputConfig;

    UPROPERTY()
    TObjectPtr<UTDRPGAbilitySystemComponent> AbilitySystemComponent;

    UTDRPGAbilitySystemComponent* GetASC();

    FVector CachedDestination = FVector::ZeroVector;
    float FollowTime = 0.0f;
    float ShortPressThreshold = 0.3f;
    bool bAutoRunning = false;
    bool bTargeting = false;

    UPROPERTY(EditDefaultsOnly)
    float AutoRunAcceptanceRadius = 50.0f;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USplineComponent> Spline;

    void AutoRun();
};
