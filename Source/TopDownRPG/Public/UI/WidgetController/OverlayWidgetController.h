// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/TDRPGWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

class UTDRPGUserWidget;
class UTDRPGAbilitySystemComponent;

struct FTDRPGAbilityInfo;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag MessageTag = FGameplayTag();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Message = FText();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UTDRPGUserWidget> MessageWidget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UTexture2D> Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPPercentChangedSignature, float, NewValue);

UCLASS(BlueprintType, Blueprintable)
class TOPDOWNRPG_API UOverlayWidgetController : public UTDRPGWidgetController
{
	GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnHealthChanged;    
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnMaxHealthChanged;
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnManaChanged;    
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnMaxManaChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FMessageWidgetRowSignature MessageWidgetRowDelegate;

    UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
    FOnXPPercentChangedSignature OnXPPercentChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
    FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;
    
protected:    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WidgetData")
    TObjectPtr<UDataTable> MessageWidgetDataTable;

    template<typename T>
    T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

    void OnXPChanged(int32 NewXP);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
    return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
