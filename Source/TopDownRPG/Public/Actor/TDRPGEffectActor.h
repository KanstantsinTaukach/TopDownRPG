// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDRPGEffectActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class TOPDOWNRPG_API ATDRPGEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDRPGEffectActor();

    UFUNCTION()
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    
protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> SphereComponent;
};
