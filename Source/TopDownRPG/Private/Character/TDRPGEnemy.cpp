// Copyright K.Taukach

#include "Character/TDRPGEnemy.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "TopDownRPG/TopDownRPG.h"
#include "UI/Widget/TDRPGUserWidget.h"

ATDRPGEnemy::ATDRPGEnemy()
{
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    AbilitySystemComponent = CreateDefaultSubobject<UTDRPGAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UTDRPGAttributeSet>("AttributeSet");

    HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
    HealthBar->SetupAttachment(GetRootComponent());
}

void ATDRPGEnemy::BeginPlay()
{
    Super::BeginPlay();

    InitAbilityActorInfo();

    if (UTDRPGUserWidget* TDRPGUserWidget = Cast<UTDRPGUserWidget>(HealthBar->GetUserWidgetObject()))
    {
        TDRPGUserWidget->SetWidgetController(this);
    }   
    
    if(const UTDRPGAttributeSet* TDRPGAttributeSet = Cast<UTDRPGAttributeSet>(AttributeSet))
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
        {
            OnHealthChanged.Broadcast(Data.NewValue);
        });
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetMaxHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
        {
            OnMaxHealthChanged.Broadcast(Data.NewValue);
        });

        OnHealthChanged.Broadcast(TDRPGAttributeSet->GetHealth());
        OnMaxHealthChanged.Broadcast(TDRPGAttributeSet->GetMaxHealth());
    }    
}

void ATDRPGEnemy::InitAbilityActorInfo()
{
    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    Cast<UTDRPGAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

    InitializeDefaultAttributes();
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
