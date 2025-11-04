// Copyright K.Taukach

#include "TDRPGGameplayTags.h"
#include "GameplayTagsManager.h"

FTDRPGGameplayTags FTDRPGGameplayTags::GameplayTags;

void FTDRPGGameplayTags::InitializeNativeGameplayTags()
{
    UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Block Chance"));
}