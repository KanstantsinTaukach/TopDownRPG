// Copyright K.Taukach

#include "TDRPGAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "TDRPGGameplayTags.h"

UTDRPGAssetManager& UTDRPGAssetManager::Get()
{
    check(GEngine);

    UTDRPGAssetManager* TDRPGAssetManager = Cast<UTDRPGAssetManager>(GEngine->AssetManager);
    return *TDRPGAssetManager;
}

void UTDRPGAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    FTDRPGGameplayTags::InitializeNativeGameplayTags();

    // This is required to use Target Data
    UAbilitySystemGlobals::Get().InitGlobalData();
}