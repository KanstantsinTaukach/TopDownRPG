// Copyright K.Taukach

#include "TDRPGGameplayTags.h"
#include "GameplayTagsManager.h"

FTDRPGGameplayTags FTDRPGGameplayTags::GameplayTags;

void FTDRPGGameplayTags::InitializeNativeGameplayTags()
{
    GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Block Chance"));
}