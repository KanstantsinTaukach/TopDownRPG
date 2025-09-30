// Copyright K.Taukach

#include "Player/TDRPGPlayerState.h"

#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"

ATDRPGPlayerState::ATDRPGPlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UTDRPGAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UTDRPGAttributeSet>("AttributeSet");
    
    NetUpdateFrequency = 100.0f;    
}

UAbilitySystemComponent* ATDRPGPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}