// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FTDRPGAbilityInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AbilityTag = FGameplayTag();

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag InputTag = FGameplayTag();

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag StatusTag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag CooldownTag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<const UTexture2D> Icon = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 LevelRequirement = 99;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UGameplayAbility> Ability;
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
