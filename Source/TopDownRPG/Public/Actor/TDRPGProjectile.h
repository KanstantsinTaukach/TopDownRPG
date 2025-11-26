// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDRPGProjectile.generated.h"

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

protected:
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> SphereComponent;
};
