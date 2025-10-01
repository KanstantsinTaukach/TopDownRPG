// Copyright K.Taukach

#include "Actor/TDRPGEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

ATDRPGEffectActor::ATDRPGEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    SetRootComponent(StaticMeshComponent);
    
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetupAttachment(GetRootComponent());
}

void ATDRPGEffectActor::BeginPlay()
{
    Super::BeginPlay();

    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATDRPGEffectActor::OnOverlap);
    SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATDRPGEffectActor::EndOverlap);
}

void ATDRPGEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack!
    
    if(IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
    {
        const UTDRPGAttributeSet* TDRPGAttributeSet = Cast<UTDRPGAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UTDRPGAttributeSet::StaticClass()));
        
        UTDRPGAttributeSet* MutableTDRPGAttibuteSet = const_cast<UTDRPGAttributeSet*>(TDRPGAttributeSet);
        MutableTDRPGAttibuteSet->SetHealth(TDRPGAttributeSet->GetHealth() + 25.0f);
        Destroy();
    }
}

void ATDRPGEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    
}

