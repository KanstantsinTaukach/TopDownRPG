// Copyright K.Taukach

#include "AbilitySystem/Abilities/TDRPGFireBolt.h"

FString UTDRPGFireBolt::GetDescription(int32 Level)
{
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);
    const int32 ScaledDamage = static_cast<int32>(Damage.GetValueAtLevel(Level));
    
    if(Level == 1)
    {
        return FString::Printf(TEXT(
            "<Title>FIRE BOLT</>\n\n"
            
            "<Small>Level: </><Level>%d</>\n"
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
            
            "<Default>Launches a bolt of fire, exploding on impact and dealing: </>"
            "<Damage>%d</><Default> fire damage with a chance to burn</>"),
            Level, ManaCost, Cooldown, ScaledDamage);
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
            Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), ScaledDamage);
    }
}

FString UTDRPGFireBolt::GetNextLevelDescription(int32 Level)
{
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);
    const int32 ScaledDamage = static_cast<int32>(Damage.GetValueAtLevel(Level));
    
    return FString::Printf(TEXT(
        "<Title>NEXT LEVEL</>\n\n"
        
        "<Small>Level: </><Level>%d</>\n"
        "<Small>ManaCost: </><ManaCost>%.1f</>\n"
        "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
        
        "<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
        "<Damage>%d</><Default> fire damage with a chance to burn</>"),
        Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), ScaledDamage);
}