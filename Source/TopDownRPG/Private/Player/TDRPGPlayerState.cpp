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
    DOREPLIFETIME(ATDRPGPlayerState, XP);
    DOREPLIFETIME(ATDRPGPlayerState, AttributePoints);
    DOREPLIFETIME(ATDRPGPlayerState, SpellPoints);
}

UAbilitySystemComponent* ATDRPGPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void ATDRPGPlayerState::SetXP(int32 NewXP)
{
    XP = NewXP;
    OnXPChangedDelegate.Broadcast(XP);
}

void ATDRPGPlayerState::SetLevel(int32 NewLevel)
{
    Level = NewLevel;
    OnLevelChangedDelegate.Broadcast(Level);
}

void ATDRPGPlayerState::SetAttributePoints(int32 NewAttributePoints)
{
    AttributePoints = NewAttributePoints;
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDRPGPlayerState::SetSpellPoints(int32 NewSpellPoints)
{
    SpellPoints = NewSpellPoints;
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ATDRPGPlayerState::AddToXP(int32 InXP)
{
    XP += InXP;
    OnXPChangedDelegate.Broadcast(XP);
}

void ATDRPGPlayerState::AddToLevel(int32 InLevel)
{
    Level += InLevel;
    OnLevelChangedDelegate.Broadcast(Level);
}

void ATDRPGPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
    AttributePoints += InAttributePoints;
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDRPGPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
    SpellPoints += InSpellPoints;
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ATDRPGPlayerState::OnRep_XP(int32 OldXP)
{
    OnXPChangedDelegate.Broadcast(XP);
}

void ATDRPGPlayerState::OnRep_Level(int32 OldLevel)
{
    OnLevelChangedDelegate.Broadcast(Level);
}

void ATDRPGPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDRPGPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}