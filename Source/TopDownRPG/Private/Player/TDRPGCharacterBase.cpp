// Copyright K.Taukach

#include "Player/TDRPGCharacterBase.h"

ATDRPGCharacterBase::ATDRPGCharacterBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATDRPGCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}