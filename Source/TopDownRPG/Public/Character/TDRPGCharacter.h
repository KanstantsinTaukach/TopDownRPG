// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Character/TDRPGCharacterBase.h"
#include "TDRPGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOPDOWNRPG_API ATDRPGCharacter : public ATDRPGCharacterBase
{
	GENERATED_BODY()

public:
    ATDRPGCharacter();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    /** Combat Interface */
    virtual int32 GetPlayerLevel() const override;
    /** end Combat Interface */
    
protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> SpringArmComponent;
    
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> CameraComponent;

private:
    virtual void InitAbilityActorInfo() override;
};
