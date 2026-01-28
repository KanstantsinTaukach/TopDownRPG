// Copyright K.Taukach

#include "Actor/TDRPGProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
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
    if(!bHit && !HasAuthority())
    {
        PlayImpactEffects();
    }
    
    Super::Destroyed();
}

void ATDRPGProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(DamageEffectSpecHandle.Data.IsValid() && DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
    {
        return;
    }
    if(!bHit)
    {
        PlayImpactEffects();
    }    

    if(HasAuthority())
    {
        UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
        if(TargetASC)
        {
            TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
        }        
        Destroy();
    }
    else
    {
        bHit = true;
    }
}

void ATDRPGProjectile::PlayImpactEffects() const
{
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
    
    if(LoopingSoundComponent)
    {
        LoopingSoundComponent->Stop(); 
    }    
}