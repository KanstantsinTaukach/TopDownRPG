// Copyright K.Taukach

#include "Character/TDRPGEnemy.h"

#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "TopDownRPG/TopDownRPG.h"

ATDRPGEnemy::ATDRPGEnemy()
{
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    AbilitySystemComponent = CreateDefaultSubobject<UTDRPGAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UTDRPGAttributeSet>("AttributeSet");
}

void ATDRPGEnemy::BeginPlay()
{
    Super::BeginPlay();

    AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ATDRPGEnemy::HighlightActor()
{
    GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

    WeaponSkeletalMesh->SetRenderCustomDepth(true);
    WeaponSkeletalMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ATDRPGEnemy::UnHighlightActor()
{
    GetMesh()->SetRenderCustomDepth(false);

    WeaponSkeletalMesh->SetRenderCustomDepth(false);
}