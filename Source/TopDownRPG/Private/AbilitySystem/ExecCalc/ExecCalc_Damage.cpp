// Copyright K.Taukach

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "TDRPGGameplayTags.h"
#include "AbilitySystem/TDRPGAttributeSet.h"

struct TDRPGDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    
    TDRPGDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, Armor, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDRPGAttributeSet, BlockChance, Target, false);
    }
};

static const TDRPGDamageStatics DamageStatics()
{
    static TDRPGDamageStatics DStatics;
    return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
    RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
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

    // Get Damage Set by Caller Magnitude
    float Damage = Spec.GetSetByCallerMagnitude(FTDRPGGameplayTags::Get().Damage);

    // Capture BlockChance on Target, and determine if there was a successful Block
    // If Block, halve the damage.

    float TargetBlockChance = 0;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParams, TargetBlockChance);
    TargetBlockChance = FMath::Max<float>(0, TargetBlockChance);
    
    const bool bBlocked = FMath::FRandRange(0.0f, 100.0f) < TargetBlockChance;
    Damage = bBlocked ? Damage / 2.0f : Damage;
    
    const FGameplayModifierEvaluatedData EvaluatedData(UTDRPGAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}