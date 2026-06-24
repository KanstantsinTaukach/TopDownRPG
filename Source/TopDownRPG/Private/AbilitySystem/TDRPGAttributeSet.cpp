// Copyright K.Taukach

#include "AbilitySystem/TDRPGAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "TDRPGAbilitySystemLibrary.h"
#include "Interaction/TDRPGCombatInterface.h"
#include "Net/UnrealNetwork.h"
#include "TDRPGGameplayTags.h"
#include "Player/TDRPGPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/TDRPGLogChannels.h"

UTDRPGAttributeSet::UTDRPGAttributeSet()
{
    const FTDRPGGameplayTags GameplayTags = FTDRPGGameplayTags::Get();

    TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
    
    TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void UTDRPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);       

    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, Strength, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, Health, COND_None, REPNOTIFY_Always);    
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, Mana, COND_None, REPNOTIFY_Always);    
    
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, Armor, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDRPGAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
}

void UTDRPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if(Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    if(Attribute == GetManaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
    }
}

void UTDRPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FEffectProperties Props;
    SetEffectProperties(Data, Props);

    if(Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
    }
    if(Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
    }
    if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
    {
        const float LocalIncomingDamage = GetIncomingDamage();
        SetIncomingDamage(0.0f);
        if(LocalIncomingDamage > 0.0f)
        {
            const float NewHealth = GetHealth() - LocalIncomingDamage;
            SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

            const bool bFatal = NewHealth <= 0.0f;
            
            if(bFatal)
            {
                ITDRPGCombatInterface* CombatInterface = Cast<ITDRPGCombatInterface>(Props.TargetAvatarActor);
                if(CombatInterface)
                {
                    CombatInterface->Die();
                }
            }
            else
            {
                FGameplayTagContainer TagContainer;
                TagContainer.AddTag(FTDRPGGameplayTags::Get().Effects_HitReact);
                Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
            }

            const bool bBlock = UTDRPGAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
            const bool bCriticalHit = UTDRPGAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
            ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
        }
    }
    if(Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
    {
        const float LocalIncomingXP = GetIncomingXP();
        SetIncomingXP(0.0f);
        UE_LOG(LogTDRPG, Log, TEXT("Incoming XP: %f"), LocalIncomingXP);
    }
}

void UTDRPGAttributeSet::ShowFloatingText(FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
    if(Props.SourceCharacter != Props.TargetCharacter)
    {        
        if(ATDRPGPlayerController* PC = Cast<ATDRPGPlayerController>(Props.SourceCharacter->Controller))
        {
            PC->ShowDamageNumber(Props.TargetCharacter, Damage, bBlockedHit, bCriticalHit);
            return;
        }
        if(ATDRPGPlayerController* PC = Cast<ATDRPGPlayerController>(Props.TargetCharacter->Controller))
        {
            PC->ShowDamageNumber(Props.TargetCharacter, Damage, bBlockedHit, bCriticalHit);
        }
    }
}

void UTDRPGAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
    // Source = causer of the effect, Target = target of the effect (owner of this AS)
    
    Props.EffectContextHandle = Data.EffectSpec.GetContext();
    Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

    if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
    {
        Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();

        Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
        if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
        {
            if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
            {
                Props.SourceController = Pawn->GetController();
            }
        }

        if(Props.SourceController)
        {
            Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
        }
    }

    if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
        Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
        // same
        // Props.TargetASC = &Data.Target;
    }
}

void UTDRPGAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, Strength, OldStrength);
}

void UTDRPGAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, Intelligence, OldIntelligence);
}

void UTDRPGAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, Resilience, OldResilience);
}

void UTDRPGAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, Vigor, OldVigor);
}

void UTDRPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, Health, OldHealth);
}

void UTDRPGAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, Mana, OldMana);
}

void UTDRPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, MaxHealth, OldMaxHealth);
}

void UTDRPGAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, MaxMana, OldMaxMana);
}

void UTDRPGAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UTDRPGAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UTDRPGAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, Armor, OldArmor);
}

void UTDRPGAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UTDRPGAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, BlockChance, OldBlockChance);
}

void UTDRPGAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UTDRPGAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UTDRPGAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UTDRPGAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, FireResistance, OldFireResistance);
}

void UTDRPGAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, LightningResistance, OldLightningResistance);
}

void UTDRPGAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UTDRPGAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDRPGAttributeSet, CriticalHitResistance, OldPhysicalResistance);
}