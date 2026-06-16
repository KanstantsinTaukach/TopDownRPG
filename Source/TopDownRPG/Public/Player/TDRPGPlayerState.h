// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TDRPGPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32 /*StatValue*/)

UCLASS()
class TOPDOWNRPG_API ATDRPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATDRPGPlayerState();

    FOnPlayerStatChangedSignature OnXPChangedDelegate;
    FOnPlayerStatChangedSignature OnLevelChangedDelegate;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<ULevelUpInfo> LevelUpInfo;

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    UAttributeSet* GetAttributeSet() const { return AttributeSet; };

    FORCEINLINE int32 GetPlayerLevel() const { return Level; };    
    FORCEINLINE int32 GetXP() const { return XP; };
    
    void SetXP(int32 NewXP);
    void SetLevel(int32 NewLevel);
    
    void AddToXP(int32 InXP);
    void AddToLevel(int32 InLevel);
    
protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

private:
    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
    int32 Level = 1;
    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
    int32 XP = 0;
    
    UFUNCTION()
    void OnRep_Level(int32 OldLevel);
    UFUNCTION()
    void OnRep_XP(int32 OldXP);
};
