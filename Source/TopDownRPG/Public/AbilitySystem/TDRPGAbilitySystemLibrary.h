// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TDRPGAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UAttributeWidgetController;
class UOverlayWidgetController;

struct FGameplayEffectContextHandle;

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
    static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

    UFUNCTION(BlueprintCallable, Category = "TDRPGAbilitySystemLibrary|CharacterClassDefaults")
    static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "TDRPGAbilitySystemLibrary|GameplayEffects")
    static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "TDRPGAbilitySystemLibrary|GameplayEffects")
    static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintCallable, Category = "TDRPGAbilitySystemLibrary|GameplayEffects")
    static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

    UFUNCTION(BlueprintCallable, Category = "TDRPGAbilitySystemLibrary|GameplayEffects")
    static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

    UFUNCTION(BlueprintCallable, Category = "TDRPGAbilitySystemLibrary|GameplayMechanics")
    static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

    UFUNCTION(BlueprintPure, Category = "TDRPGAbilitySystemLibrary|GameplayMechanics")
    static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

    UFUNCTION(BlueprintCallable, Category = "TDRPGAbilitySystemLibrary|XP")
    static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
};
