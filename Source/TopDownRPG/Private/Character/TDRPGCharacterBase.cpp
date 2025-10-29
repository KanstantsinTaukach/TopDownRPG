// Copyright K.Taukach

#include "Character/TDRPGCharacterBase.h"
#include "AbilitySystemComponent.h"

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

void ATDRPGCharacterBase::InitAbilityActorInfo()
{
    
}

void ATDRPGCharacterBase::InitializePrimaryAttributes() const
{
    check(IsValid(GetAbilitySystemComponent()));
    check(DefaultPrimaryAttributes);
    const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
    const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.0f, ContextHandle);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}