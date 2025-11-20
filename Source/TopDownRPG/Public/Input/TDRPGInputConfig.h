// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TDRPGInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FTDRPGInputAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    const class UInputAction* InputAction = nullptr;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag InputTag = FGameplayTag();
};

UCLASS()
class TOPDOWNRPG_API UTDRPGInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FTDRPGInputAction> AbilityInputActions;

    const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
};
