// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDRPGCharacterBase.generated.h"

UCLASS()
class TOPDOWNRPG_API ATDRPGCharacterBase : public ACharacter
{
    GENERATED_BODY()

public:
    ATDRPGCharacterBase();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;
};
