// Copyright K.Taukach

#include "TDRPGAssetManager.h"
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
}