// Copyright K.Taukach

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/TDRPGAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/TDRPGCombatInterface.h"
#include "TDRPGGameplayTags.h"
#include "AbilitySystem/TDRPGAttributeSet.h"

struct TDRPGDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

    DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

    TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
    
    TDRPGDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, BlockChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, Armor, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, ArmorPenetration, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, CriticalHitChance, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, CriticalHitDamage, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, CriticalHitResistance, Target, false);

        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, FireResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, LightningResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, ArcaneResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, PhysicalResistance, Target, false);

        const FTDRPGGameplayTags& Tags = FTDRPGGameplayTags::Get();
        TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
        TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
        TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
        TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
        TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
        TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
        
        TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
        TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
        TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
        TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
    }
};

static const TDRPGDamageStatics DamageStatics()
{
    static TDRPGDamageStatics DStatics;
    return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
    RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
    
    RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

    ITDRPGCombatInterface* SourceCombatInterface = Cast<ITDRPGCombatInterface>(SourceAvatar);
    ITDRPGCombatInterface* TargetCombatInterface = Cast<ITDRPGCombatInterface>(TargetAvatar);

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
    
    FAggregatorEvaluateParameters EvaluationParams;
    EvaluationParams.SourceTags = SourceTags;
    EvaluationParams.TargetTags = TargetTags;

    // Get Damage Set by Caller Magnitude.
    float Damage = 0.0f;
    for(const TTuple<FGameplayTag, FGameplayTag> Pair : FTDRPGGameplayTags::Get().DamageTypesToResistances)
    {
        const FGameplayTag DamageTypeTag = Pair.Key;
        const FGameplayTag ResistanceTag = Pair.Value;

        checkf(TDRPGDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage,"), *ResistanceTag.ToString());
        const FGameplayEffectAttributeCaptureDefinition CaptureDef = TDRPGDamageStatics().TagsToCaptureDefs[ResistanceTag];

        float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);
        
        float Resistance = 0.0f;
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParams, Resistance);
        Resistance = FMath::Clamp(Resistance, 0.0f, 100.0f);

        DamageTypeValue *= (100.0f - Resistance) / 100.0f;
        
        Damage += DamageTypeValue;
    }

    // Capture BlockChance on Target, and determine if there was a successful Block. 
    float TargetBlockChance = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParams, TargetBlockChance);
    TargetBlockChance = FMath::Max<float>(0.0f, TargetBlockChance);
    
    const bool bBlocked = FMath::FRandRange(0.0f, 100.0f) < TargetBlockChance;

    FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
    UTDRPGAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
    
    // If Block, halve the damage.
    Damage = bBlocked ? Damage / 2.0f : Damage;
    
    float TargetArmor = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParams, TargetArmor);
    TargetArmor = FMath::Max<float>(0.0f, TargetArmor);

    float SourceArmorPenetration = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParams, SourceArmorPenetration);
    SourceArmorPenetration = FMath::Max<float>(0.0f, SourceArmorPenetration);

    const UCharacterClassInfo* CharacterClassInfo = UTDRPGAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);

    const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetrationCurve"), FString());
    const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());    
    
    // ArmorPenetration ignores a percentage of the Target's Armor.
    const float EffectiveArmor = TargetArmor * (100.0f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.0f;
    
    const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmorCurve"), FString());
    const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
    
    // Armor ignores a percentage of incoming Damage.
    Damage *= (100.0 - EffectiveArmor * EffectiveArmorCoefficient) / 100.0f;

    float SourceCriticalHitChance = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParams, SourceCriticalHitChance);
    SourceCriticalHitChance = FMath::Max<float>(0.0f, SourceCriticalHitChance);

    float TargetCriticalHitResistance = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParams, TargetCriticalHitResistance);
    TargetCriticalHitResistance = FMath::Max<float>(0.0f, TargetCriticalHitResistance);
    
    const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistanceCurve"), FString());
    const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());    

    // Critical Hit resistance reduces Critical Hit Chance by a certain percentage
    const float EffectiveCriticalHitChance = SourceCriticalHitChance  - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;

    const bool bCriticalHit = FMath::FRandRange(0.0f, 100.0f) < EffectiveCriticalHitChance;

    UTDRPGAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
    
    if(bCriticalHit)
    {
        float SourceCriticalHitDamage = 0.0f;
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParams, SourceCriticalHitDamage);
        SourceCriticalHitDamage = FMath::Max<float>(0.0f, SourceCriticalHitDamage);

        // Double damage plus a bonus if critical hit
        Damage = Damage * 2.0f + SourceCriticalHitDamage;
    }
    
    const FGameplayModifierEvaluatedData EvaluatedData(UTDRPGAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}