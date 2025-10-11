// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDRPGEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class TOPDOWNRPG_API ATDRPGEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDRPGEffectActor();
    
protected:
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);
    
    UPROPERTY(EditAnywhere, Category = "Applied Effects")
    TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
};
