// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TDRPGCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class TOPDOWNRPG_API ATDRPGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ATDRPGCharacterBase();
    
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    UAttributeSet* GetAttributeSet() const { return AttributeSet; };

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;
};
