// Copyright K.Taukach

#include "AbilitySystem/MMC/MMC_MaxMana.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "Interaction/TDRPGCombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
    IntelligenceDef.AttributeToCapture = UTDRPGAttributeSet::GetIntelligenceAttribute();
    IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    IntelligenceDef.bSnapshot = false;

    RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParams;
    EvaluationParams.SourceTags = SourceTags;
    EvaluationParams.TargetTags = TargetTags;

    float Intelligence = 0.0f;
    GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParams, Intelligence);
    Intelligence = FMath::Max(Intelligence, 0.0f);

    const ITDRPGCombatInterface* CombatInterface = Cast<ITDRPGCombatInterface>(Spec.GetContext().GetSourceObject());
    const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

    return 50.0f + 2.0f * Intelligence + 10.0f * PlayerLevel;
}