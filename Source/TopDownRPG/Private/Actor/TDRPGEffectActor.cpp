// Copyright K.Taukach

#include "Actor/TDRPGEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

ATDRPGEffectActor::ATDRPGEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void ATDRPGEffectActor::BeginPlay()
{
    Super::BeginPlay();
}

void ATDRPGEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
    /** We can use this instead of UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
    IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(Target);
    if(ASInterface)
    {
        UAbilitySystemComponent* TargetASC = ASInterface->GetAbilitySystemComponent();
    }    
    */    
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
    if(TargetASC == nullptr) return;

    check(GameplayEffectClass);
    FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
    EffectContextHandle.AddSourceObject(this);
    const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);
    TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}
