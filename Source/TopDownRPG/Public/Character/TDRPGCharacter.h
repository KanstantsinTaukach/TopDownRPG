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
    
protected:
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArmComponent;
    
    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComponent;	
};
