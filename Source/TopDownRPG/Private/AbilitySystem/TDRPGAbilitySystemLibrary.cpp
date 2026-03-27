// Copyright K.Taukach


#include "AbilitySystem/TDRPGAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "TDRPGAbilityTypes.h"
#include "Interaction/TDRPGCombatInterface.h"
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
    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
    if(!CharacterClassInfo) return;
    
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

void UTDRPGAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
    if(CharacterClassInfo == nullptr) return;
    
    for(TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        ASC->GiveAbility(AbilitySpec);
    }

    const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
    for(TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
    {
        ITDRPGCombatInterface* CombatInterface = Cast<ITDRPGCombatInterface>(ASC->GetAvatarActor());
        if(CombatInterface)
        {
            FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
            ASC->GiveAbility(AbilitySpec);
        }
    }
}

UCharacterClassInfo* UTDRPGAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
    ATDRPGGameModeBase* TDRPGGameMode = Cast<ATDRPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if(TDRPGGameMode == nullptr) return nullptr;
    return TDRPGGameMode->CharacterClassInfo;
}

bool UTDRPGAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    if(const FTDRPGGameplayEffectContext* TDRPGEffectContext = static_cast<const FTDRPGGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return TDRPGEffectContext->IsBlockedHit();
    }
    return false;
}

bool UTDRPGAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    if(const FTDRPGGameplayEffectContext* TDRPGEffectContext = static_cast<const FTDRPGGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return TDRPGEffectContext->IsCriticalHit();
    }
    return false;
}

void UTDRPGAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
    if(FTDRPGGameplayEffectContext* TDRPGEffectContext = static_cast<FTDRPGGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        TDRPGEffectContext->SetIsBlockedHit(bInIsBlockedHit);
    }
}

void UTDRPGAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
    if(FTDRPGGameplayEffectContext* TDRPGEffectContext = static_cast<FTDRPGGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        TDRPGEffectContext->SetIsCriticalHit(bInIsCriticalHit);
    }
}

void UTDRPGAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
    const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
    FCollisionQueryParams SphereParams;
    SphereParams.AddIgnoredActors(ActorsToIgnore);
    
    if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        TArray<FOverlapResult> Overlaps;
        World->OverlapMultiByObjectType(Overlaps,
            SphereOrigin,
            FQuat::Identity,
            FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
            FCollisionShape::MakeSphere(Radius),
            SphereParams);

        for(FOverlapResult& Overlap : Overlaps)
        {
            if(Overlap.GetActor()->Implements<UTDRPGCombatInterface>() && !ITDRPGCombatInterface::Execute_IsDead(Overlap.GetActor()))
            {
                OutOverlappingActors.AddUnique(Overlap.GetActor());
            }
        }
    }
}

bool UTDRPGAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
    const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
    const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
    const bool bFriends = bBothArePlayers || bBothAreEnemies;
    return !bFriends;
}