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

void ATDRPGCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
    check(IsValid(GetAbilitySystemComponent()));
    check(GameplayEffectClass);
    const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
    const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ATDRPGCharacterBase::InitializeDefaultAttributes() const
{
    ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
    ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
}