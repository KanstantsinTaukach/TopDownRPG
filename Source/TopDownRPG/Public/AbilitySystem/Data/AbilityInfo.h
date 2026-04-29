// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FTDRPGAbilityInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AbilityTag = FGameplayTag();

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag InputTag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<const UTexture2D> Icon = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
};

UCLASS()
class TOPDOWNRPG_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
    FTDRPGAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
    TArray<FTDRPGAbilityInfo> AbilityInformation;
};
