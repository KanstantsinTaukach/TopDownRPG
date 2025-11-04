// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilities/Private/Tests/GameplayCueTests.h"

/**
 * TDRPG GameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FTDRPGGameplayTags
{
public:
    static const FTDRPGGameplayTags& Get() { return GameplayTags; }
    static void InitializeNativeGameplayTags();

    FGameplayTag Attributes_Secondary_Armor;

protected:

private:
    static FTDRPGGameplayTags GameplayTags;
};