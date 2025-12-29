// Copyright K.Taukach

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "TDRPGGameplayTags.h"
#include "AbilitySystem/TDRPGAttributeSet.h"

struct TDRPGDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
    
    TDRPGDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, BlockChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, Armor, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, ArmorPenetration, Source, false);
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
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
    
    FAggregatorEvaluateParameters EvaluationParams;
    EvaluationParams.SourceTags = SourceTags;
    EvaluationParams.TargetTags = TargetTags;

    // Get Damage Set by Caller Magnitude.
    float Damage = Spec.GetSetByCallerMagnitude(FTDRPGGameplayTags::Get().Damage);

    // Capture BlockChance on Target, and determine if there was a successful Block. 
    float TargetBlockChance = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParams, TargetBlockChance);
    TargetBlockChance = FMath::Max<float>(0.0f, TargetBlockChance);
    
    const bool bBlocked = FMath::FRandRange(0.0f, 100.0f) < TargetBlockChance;
    
    // If Block, halve the damage.
    Damage = bBlocked ? Damage / 2.0f : Damage;

    float TargetArmor = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParams, TargetArmor);
    TargetArmor = FMath::Max<float>(0.0f, TargetArmor);

    float SourceArmorPenetration = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParams, SourceArmorPenetration);
    SourceArmorPenetration = FMath::Max<float>(0.0f, SourceArmorPenetration);

    // ArmorPenetration ignores a percentage of the Target's Armor.
    const float EffectiveArmor = TargetArmor *= (100.0f - SourceArmorPenetration * 0.4) / 100.0f;
    // Armor ignores a percentage of incoming Damage.
    Damage *= (100.0 - EffectiveArmor * 0.3) / 100.0f;    
    
    const FGameplayModifierEvaluatedData EvaluatedData(UTDRPGAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}