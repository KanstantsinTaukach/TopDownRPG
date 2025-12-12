// Copyright K.Taukach

#include "AbilitySystem/Abilities/TDRPGProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/TDRPGProjectile.h"
#include "Interaction/TDRPGCombatInterface.h"
#include "TDRPGGameplayTags.h"

void UTDRPGProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);        
}

void UTDRPGProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
    if(!bIsServer) return;

    ITDRPGCombatInterface* CombatInterface = Cast<ITDRPGCombatInterface>(GetAvatarActorFromActorInfo());
    if(CombatInterface)
    {
        const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
        FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
        Rotation.Pitch = 0.0f;
        
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

        const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
        const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

        const FTDRPGGameplayTags GameplayTags = FTDRPGGameplayTags::Get();
        const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);        
        Projectile->DamageEffectSpecHandle = SpecHandle;
        
        Projectile->FinishSpawning(SpawnTransform);
    }
}