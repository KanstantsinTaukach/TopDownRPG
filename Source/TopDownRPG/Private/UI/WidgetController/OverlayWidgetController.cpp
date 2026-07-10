// Copyright K.Taukach

#include "UI/WidgetController/OverlayWidgetController.h"

#include "LevelUpInfo.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/TDRPGPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    const UTDRPGAttributeSet* TDRPGAttributeSet = CastChecked<UTDRPGAttributeSet>(AttributeSet);
    
    OnHealthChanged.Broadcast(TDRPGAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(TDRPGAttributeSet->GetMaxHealth());

    OnManaChanged.Broadcast(TDRPGAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(TDRPGAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{    
    ATDRPGPlayerState* TDRPGPlayerState = CastChecked<ATDRPGPlayerState>(PlayerState);
    TDRPGPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
    TDRPGPlayerState->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel)
    {
        OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
    });
    
    const UTDRPGAttributeSet* TDRPGAttributeSet = CastChecked<UTDRPGAttributeSet>(AttributeSet);

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

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnManaChanged.Broadcast(Data.NewValue);
    });
    
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDRPGAttributeSet->GetMaxManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
        OnMaxManaChanged.Broadcast(Data.NewValue);
    });

    if(UTDRPGAbilitySystemComponent* TDRPGASC = Cast<UTDRPGAbilitySystemComponent>(AbilitySystemComponent))
    {
        if(TDRPGASC->IsStartupAbilitiesGiven())
        {
            OnInitializeStartupAbilities(TDRPGASC);
        }
        else
        {
            TDRPGASC->AbilitiesGiven.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
        }        
    
        TDRPGASC->EffectAssetTags.AddLambda(
        [this](const FGameplayTagContainer& AssetTags)
        {
            for(const FGameplayTag& Tag : AssetTags)
            {
                // "Message.HealthPotion".MatchesTag("Message") will return true, "Message".MatchesTag("Message.HealthPotion") will return false

                FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
                if(Tag.MatchesTag(MessageTag))
                {
                    const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
                    //if(Row != nullptr)
                    {
                        MessageWidgetRowDelegate.Broadcast(*Row);
                    }                    
                }
            }
        });
    }
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UTDRPGAbilitySystemComponent* TDRPGAbilitySystemComponent)
{
    // TODO Get information about all given abilities, look up their Ability Info, and broadcast it to widgets.
    if(!TDRPGAbilitySystemComponent->IsStartupAbilitiesGiven()) return;

    FForEachAbility BroadcastDelegate;
    BroadcastDelegate.BindLambda([this, TDRPGAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
    {
        // TODO need a way to figure out the ability tag for a given ability spec.
        FTDRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(TDRPGAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
        Info.InputTag = TDRPGAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
        AbilityInfoDelegate.Broadcast(Info);
    });
    TDRPGAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
    const ATDRPGPlayerState* TDRPGPlayerState = CastChecked<ATDRPGPlayerState>(PlayerState);
    const ULevelUpInfo* LevelUpInfo = TDRPGPlayerState->LevelUpInfo;
    checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out TDRTGPlayerState Blueprint."));

    const int32 PlayerLevel = LevelUpInfo->FindLevelForXP(NewXP);
    const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

    if(PlayerLevel <= MaxLevel && PlayerLevel > 0)
    {
        const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[PlayerLevel].LevelUpRequirement;
        const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[PlayerLevel - 1].LevelUpRequirement;
        
        const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
        const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

        const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
        OnXPPercentChanged.Broadcast(XPBarPercent);
    }
}