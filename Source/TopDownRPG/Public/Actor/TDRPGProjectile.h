// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "TDRPGProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TOPDOWNRPG_API ATDRPGProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDRPGProjectile();

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;
    virtual void Destroyed() override;

    UFUNCTION()
    void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> SphereComponent;
    
private:
    bool bHit = false;

    UPROPERTY(EditDefaultsOnly)
    float LifeSpan = 5.0f;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UNiagaraSystem> ImpactEffect;

    UPROPERTY(EditAnywhere)
    TObjectPtr<USoundBase> ImpactSound;

    UPROPERTY(EditAnywhere)
    TObjectPtr<USoundBase> LoopingSound;

    UPROPERTY()
    TObjectPtr<UAudioComponent> LoopingSoundComponent;

    void PlayImpactEffects() const;
};
