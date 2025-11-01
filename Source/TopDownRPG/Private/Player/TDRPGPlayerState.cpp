// Copyright K.Taukach

#include "Player/TDRPGPlayerState.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

ATDRPGPlayerState::ATDRPGPlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UTDRPGAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UTDRPGAttributeSet>("AttributeSet");
    
    SetNetUpdateFrequency(100.0f);    
}

void ATDRPGPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATDRPGPlayerState, Level);
}

UAbilitySystemComponent* ATDRPGPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void ATDRPGPlayerState::OnRep_Level(int32 OldLevel)
{
}