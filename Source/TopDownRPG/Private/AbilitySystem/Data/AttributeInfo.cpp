// Copyright K.Taukach

#include "AbilitySystem/Data/AttributeInfo.h"
#include "TopDownRPG/TDRPGLogChannels.h"

FTDRPGAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
    for(const FTDRPGAttributeInfo& Info : AttributeInformation)
    {
        if(Info.AttributeTag.MatchesTagExact(AttributeTag))
        {
            return Info;
        }
    }

    if(bLogNotFound)
    {
        UE_LOG(LogTDRPG, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
    }
    
    return FTDRPGAttributeInfo();
}