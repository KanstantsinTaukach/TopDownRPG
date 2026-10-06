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

    const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
    FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
    EffectContextHandle.SetAbility(this);
    EffectContextHandle.AddSourceObject(Projectile);
    TArray<TWeakObjectPtr<AActor>> Actors;
    Actors.Add(Projectile);
    EffectContextHandle.AddActors(Actors);
    FHitResult HitResult;
    HitResult.Location = ProjectileTargetLocation;
    EffectContextHandle.AddHitResult(HitResult);
        
    const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

    for(auto& Pair : DamageTypes)
    {
        const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
    }
          
    Projectile->DamageEffectSpecHandle = SpecHandle;
        
    Projectile->FinishSpawning(SpawnTransform);
}

FString UTDRPGProjectileSpell::GetDescription(int32 Level)
{
    int32 Damage = 0;
    if(const FScalableFloat* FireDamage = DamageTypes.Find(FTDRPGGameplayTags::Get().Damage_Fire))
    {
        Damage = static_cast<int32>(FireDamage->GetValueAtLevel(Level));
    }
    
    if(Level == 1)
    {
        return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n"
                                "<Default>Launches a bolt of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n"
                                "<Small>Level: </><Level>%d</>"), Damage, Level);
    }
    else
    {
        return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n"
                                "<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n"
                                "<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectiles), Damage, Level);
    }
    
    
}

FString UTDRPGProjectileSpell::GetNextLevelDescription(int32 Level)
{
    int32 Damage = 0;
    if(const FScalableFloat* FireDamage = DamageTypes.Find(FTDRPGGameplayTags::Get().Damage_Fire))
    {
        Damage = static_cast<int32>(FireDamage->GetValueAtLevel(Level));
    }
    return FString::Printf(TEXT("<Title>NEXT LEVEL</>\n\n"
                                "<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n"
                                "<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectiles), Damage, Level);
}