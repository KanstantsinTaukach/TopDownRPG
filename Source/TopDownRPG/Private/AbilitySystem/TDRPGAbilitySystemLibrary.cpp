// Copyright K.Taukach


#include "AbilitySystem/TDRPGAbilitySystemLibrary.h"
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