// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TDRPGAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UAttributeWidgetController;
class UOverlayWidgetController;

UCLASS()
class TOPDOWNRPG_API UTDRPGAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "TDRPGAbilitySystemLibrary|WidgetController")
    static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "TDRPGAbilitySystemLibrary|WidgetController")
    static UAttributeWidgetController* GetAttributeWidgetController (const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "TDRPGAbilitySystemLibrary|CharacterClassDefaults")
    static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

    UFUNCTION(BlueprintCallable, Category = "TDRPGAbilitySystemLibrary|CharacterClassDefaults")
    static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

    UFUNCTION(BlueprintCallable, Category = "TDRPGAbilitySystemLibrary|CharacterClassDefaults")
    static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
};
