// Copyright K.Taukach

#include "AbilitySystem/Abilities/TDRPGProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/TDRPGProjectile.h"
#include "Interaction/TDRPGCombatInterface.h"

void UTDRPGProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);        
}

void UTDRPGProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
    if(!bIsServer) return;

    const FVector SocketLocation = ITDRPGCombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
    FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
    if(bOverridePitch)
    {
        Rotation.Pitch = PitchOverride;
    }
        
    FTransform SpawnTransform;
    SpawnTransform.SetLocation(SocketLocation);
    SpawnTransform.SetRotation(Rotation.Quaternion());
        
    AActor* OwningActor = GetOwningActorFromActorInfo();
        
    ATDRPGProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATDRPGProjectile>(
        ProjectileClass,
        SpawnTransform,
        OwningActor,
        Cast<APawn>(OwningActor),
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    
    Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
        
    Projectile->FinishSpawning(SpawnTransform);
}