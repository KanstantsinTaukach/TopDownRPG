// Copyright K.Taukach

#include "AbilitySystem/Abilities/TDRPGFireBolt.h"
#include "TDRPGGameplayTags.h"

FString UTDRPGFireBolt::GetDescription(int32 Level)
{
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);
    const int32 Damage = static_cast<int32>(GetDamageByDamageType(Level, FTDRPGGameplayTags::Get().Damage_Fire));
    
    if(Level == 1)
    {
        return FString::Printf(TEXT(
            "<Title>FIRE BOLT</>\n\n"
            
            "<Small>Level: </><Level>%d</>\n"
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
            
            "<Default>Launches a bolt of fire, exploding on impact and dealing: </>"
            "<Damage>%d</><Default> fire damage with a chance to burn</>"),
            Level, ManaCost, Cooldown, Damage);
    }
    else
    {
        return FString::Printf(TEXT(
            "<Title>FIRE BOLT</>\n\n"
            
            "<Small>Level: </><Level>%d</>\n"
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
            
            "<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
            "<Damage>%d</><Default> fire damage with a chance to burn</>"),
            Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), Damage);
    }
}

FString UTDRPGFireBolt::GetNextLevelDescription(int32 Level)
{
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);
    const int32 Damage = static_cast<int32>(GetDamageByDamageType(Level, FTDRPGGameplayTags::Get().Damage_Fire));
    
    return FString::Printf(TEXT(
        "<Title>NEXT LEVEL</>\n\n"
        
        "<Small>Level: </><Level>%d</>\n"
        "<Small>ManaCost: </><ManaCost>%.1f</>\n"
        "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
        
        "<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
        "<Damage>%d</><Default> fire damage with a chance to burn</>"),
        Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), Damage);
}