// Copyright K.Taukach

#include "Actor/TDRPGProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/TDRPGAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/TopDownRPG.h"

ATDRPGProjectile::ATDRPGProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
    SetRootComponent(SphereComponent);
    SphereComponent->SetCollisionObjectType(ECC_Projectile);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
    ProjectileMovementComponent->InitialSpeed = 550.0f;
    ProjectileMovementComponent->MaxSpeed = 550.0f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ATDRPGProjectile::BeginPlay()
{
	Super::BeginPlay();
    SetLifeSpan(LifeSpan);

    LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());

    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATDRPGProjectile::OnSphereOverlap);
}

void ATDRPGProjectile::Destroyed()
{
    if(!bHit && !HasAuthority()) OnHit();    
    Super::Destroyed();
}

void ATDRPGProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
    if(SourceAvatarActor == OtherActor) return;    
    if(!UTDRPGAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;    
    if(!bHit) OnHit();

    if(HasAuthority())
    {
        if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
        {
            DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
            UTDRPGAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
        }        
        Destroy();
    }
    else bHit = true;
}

void ATDRPGProjectile::OnHit()
{
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
    if(LoopingSoundComponent) LoopingSoundComponent->Stop();
    bHit = true;
}