// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Interaction/TDRPGPlayerInterface.h"
#include "Character/TDRPGCharacterBase.h"
#include "TDRPGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOPDOWNRPG_API ATDRPGCharacter : public ATDRPGCharacterBase, public ITDRPGPlayerInterface
{
	GENERATED_BODY()

public:
    ATDRPGCharacter();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    /** Combat Interface */
    virtual int32 GetPlayerLevel_Implementation() const override;
    /** end Combat Interface */

    /** Player Interface */
    virtual int32 GetXP_Implementation() const override;
    virtual void AddToXP_Implementation(int32 InXP) override;

    virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
    virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;

    virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
    virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;

    
    virtual void LevelUp_Implementation() override;
    virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
    virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
    /** end Player Interface */
    
protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> SpringArmComponent;
    
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> CameraComponent;

private:
    virtual void InitAbilityActorInfo() override;
};
