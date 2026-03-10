// Copyright K.Taukach

#include "Character/TDRPGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "TDRPGAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "TopDownRPG/TopDownRPG.h"

ATDRPGCharacterBase::ATDRPGCharacterBase()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetCapsuleComponent()->SetGenerateOverlapEvents(false);
    
    GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
    GetMesh()->SetGenerateOverlapEvents(true);
    
    WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMesh");
    WeaponSkeletalMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
    WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATDRPGCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void ATDRPGCharacterBase::InitAbilityActorInfo()
{
    
}

FVector ATDRPGCharacterBase::GetCombatSocketLocation_Implementation()
{
    check(WeaponSkeletalMesh);
    return WeaponSkeletalMesh->GetSocketLocation(WeaponTipSocketName);
}

void ATDRPGCharacterBase::Die()
{
    WeaponSkeletalMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
    MulticastHandleDeath();
}

void ATDRPGCharacterBase::MulticastHandleDeath_Implementation()
{
    WeaponSkeletalMesh->SetSimulatePhysics(true);
    WeaponSkeletalMesh->SetEnableGravity(true);
    WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetEnableGravity(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Dissolve();
}

void ATDRPGCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
    check(IsValid(GetAbilitySystemComponent()));
    check(GameplayEffectClass);
    FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
    ContextHandle.AddSourceObject(this);
    const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ATDRPGCharacterBase::InitializeDefaultAttributes() const
{
    ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
    ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
    ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);
}

void ATDRPGCharacterBase::AddCharacterAbilities()
{
    if(!HasAuthority()) return;

    UTDRPGAbilitySystemComponent* TDRPGASC = Cast<UTDRPGAbilitySystemComponent>(AbilitySystemComponent);
    TDRPGASC->AddCharacterAbilities(StartupAbilities);
}

void ATDRPGCharacterBase::Dissolve()
{
    if(MeshDissolveMaterialInstance)
    {
        UMaterialInstanceDynamic* MeshDynamicMaterialInstance = UMaterialInstanceDynamic::Create(MeshDissolveMaterialInstance, this);
        GetMesh()->SetMaterial(0, MeshDynamicMaterialInstance);
        StartMeshDissolveTimeline(MeshDynamicMaterialInstance);
    }
    if(WeaponDissolveMaterialInstance)
    {
        UMaterialInstanceDynamic* WeaponDynamicMaterialInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
        WeaponSkeletalMesh->SetMaterial(0, WeaponDynamicMaterialInstance);
        StartWeaponDissolveTimeline(WeaponDynamicMaterialInstance);
    }
}