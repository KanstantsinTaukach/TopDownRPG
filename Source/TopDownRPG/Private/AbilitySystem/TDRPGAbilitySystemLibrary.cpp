// Copyright K.Taukach


#include "AbilitySystem/TDRPGAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "Game/TDRPGGameModeBase.h"
#include "UI/HUD/TDRPGHUD.h"
#include "UI/WidgetController/TDRPGWidgetController.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDRPGPlayerState.h"

UOverlayWidgetController* UTDRPGAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
    if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        if(ATDRPGHUD* TDRPGHUD = Cast<ATDRPGHUD>(PC->GetHUD()))
        {
            ATDRPGPlayerState* PS = PC->GetPlayerState<ATDRPGPlayerState>();
            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            UAttributeSet* AS = PS->GetAttributeSet();

            const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
            return TDRPGHUD->GetOverlayWidgetController(WidgetControllerParams);
        }
    }

    return nullptr;
}

UAttributeWidgetController* UTDRPGAbilitySystemLibrary::GetAttributeWidgetController(const UObject* WorldContextObject)
{
    if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        if(ATDRPGHUD* TDRPGHUD = Cast<ATDRPGHUD>(PC->GetHUD()))
        {
            ATDRPGPlayerState* PS = PC->GetPlayerState<ATDRPGPlayerState>();
            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            UAttributeSet* AS = PS->GetAttributeSet();

            const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
            return TDRPGHUD->GetAttributeWidgetController(WidgetControllerParams);
        }
    }

    return nullptr;
}

void UTDRPGAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
    ATDRPGGameModeBase* TDRPGGameMode = Cast<ATDRPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if(TDRPGGameMode == nullptr) return;    
    
    UCharacterClassInfo* CharacterClassInfo = TDRPGGameMode->CharacterClassInfo;
    FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

    AActor* AvatarActor = ASC->GetAvatarActor();
    FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
    EffectContextHandle.AddSourceObject(AvatarActor);
    
    const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, EffectContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

    const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, EffectContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());
    
    const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, EffectContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}