// Copyright K.Taukach

#include "AbilitySystem/Data/AttributeInfo.h"

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
        UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
    }
    
    return FTDRPGAttributeInfo();
}