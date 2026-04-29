// Copyright K.Taukach

#include "AbilitySystem/Data/AbilityInfo.h"
#include "TopDownRPG/TDRPGLogChannels.h"

FTDRPGAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
    for(const FTDRPGAbilityInfo Info : AbilityInformation)
    {
        if(Info.AbilityTag == AbilityTag)
        {
            return Info;
        }
    }

    if(bLogNotFound)
    {
        UE_LOG(LogTDRPG, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
    }
    
    return FTDRPGAbilityInfo();
}