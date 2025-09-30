// Copyright K.Taukach

#include "Character/TDRPGCharacterBase.h"

ATDRPGCharacterBase::ATDRPGCharacterBase()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMesh");
    WeaponSkeletalMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
    WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATDRPGCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

UAbilitySystemComponent* ATDRPGCharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}