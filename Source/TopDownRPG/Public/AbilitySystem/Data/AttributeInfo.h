// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FTDRPGAttributeInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AttributeTag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AttributeName = FText();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AttributeDescription = FText();

    UPROPERTY(BlueprintReadOnly)
    float AttributeValue = 0.0f;
};

UCLASS()
class TOPDOWNRPG_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
    FTDRPGAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FTDRPGAttributeInfo> AttributeInformation;
};
