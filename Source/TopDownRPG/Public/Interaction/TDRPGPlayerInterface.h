// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TDRPGPlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTDRPGPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class TOPDOWNRPG_API ITDRPGPlayerInterface
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintNativeEvent)
    int32 GetXP() const;    
    UFUNCTION(BlueprintNativeEvent)
    void AddToXP(int32 InXP);
    
    UFUNCTION(BlueprintNativeEvent)
    int32 GetAttributePointsReward(int32 Level) const;
    UFUNCTION(BlueprintNativeEvent)
    void AddToAttributePoints(int32 InAttributePoints);
    
    UFUNCTION(BlueprintNativeEvent)
    int32 GetSpellPointsReward(int32 Level) const;
    UFUNCTION(BlueprintNativeEvent)
    void AddToSpellPoints(int32 InSpellPoints);

    UFUNCTION(BlueprintNativeEvent)
    void LevelUp();
    UFUNCTION(BlueprintNativeEvent)
    int32 FindLevelForXP(int32 InXP) const;
    UFUNCTION(BlueprintNativeEvent)
    void AddToPlayerLevel(int32 InPlayerLevel);

    UFUNCTION(BlueprintNativeEvent)
    int32 GetAttributePoints() const;
    UFUNCTION(BlueprintNativeEvent)
    int32 GetSpellPoints() const;
};
