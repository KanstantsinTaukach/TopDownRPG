// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TDRPGWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ATDRPGPlayerController;
class ATDRPGPlayerState;
class UTDRPGAbilitySystemComponent;
class UTDRPGAttributeSet;
class UAbilityInfo;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
    GENERATED_BODY()

    FWidgetControllerParams(){}
    FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
    : PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerController> PlayerController = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerState> PlayerState = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FTDRPGAbilityInfo&, Info);

UCLASS()
class TOPDOWNRPG_API UTDRPGWidgetController : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams);

    UFUNCTION(BlueprintCallable)
    virtual void BroadcastInitialValues();
    virtual void BindCallbacksToDependencies();

    UPROPERTY(BlueprintAssignable, Category = "GAS|Abilities")
    FAbilityInfoSignature AbilityInfoDelegate;

    void BroadcastAbilityInfo();
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WidgetData")
    TObjectPtr<UAbilityInfo> AbilityInfo;
    
    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<APlayerState> PlayerState;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<UAttributeSet> AttributeSet;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<ATDRPGPlayerController> TDRPGPlayerController;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<ATDRPGPlayerState> TDRPGPlayerState;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<UTDRPGAbilitySystemComponent> TDRPGAbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, Category="WidgetContoller")
    TObjectPtr<UTDRPGAttributeSet> TDRPGAttributeSet;

    ATDRPGPlayerController* GetTDRPGPlayerController();
    ATDRPGPlayerState* GetTDRPGPlayerState();
    UTDRPGAbilitySystemComponent* GetTDRPGAbilitySystemComponent();
    UTDRPGAttributeSet* GetTDRPGAttributeSet();
};
