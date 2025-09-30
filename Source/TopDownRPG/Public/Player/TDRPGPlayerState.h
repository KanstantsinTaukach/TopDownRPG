// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TDRPGPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class TOPDOWNRPG_API ATDRPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATDRPGPlayerState();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    UAttributeSet* GetAttributeSet() const { return AttributeSet; };

protected:
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;
};
