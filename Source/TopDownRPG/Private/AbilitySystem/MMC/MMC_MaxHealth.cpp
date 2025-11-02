// Copyright K.Taukach

#include "AbilitySystem/MMC/MMC_MaxHealth.h"
#include "AbilitySystem/TDRPGAttributeSet.h"
#include "Interaction/TDRPGCombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
    VigorDef.AttributeToCapture = UTDRPGAttributeSet::GetVigorAttribute();
    VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    VigorDef.bSnapshot = false;

    RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    // Gather tags from source and target
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParams;
    EvaluationParams.SourceTags = SourceTags;
    EvaluationParams.TargetTags = TargetTags;

    float Vigor = 0.0f;
    GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParams, Vigor);
    Vigor = FMath::Max<float>(Vigor, 0.0f);

    const ITDRPGCombatInterface* CombatInterface = Cast<ITDRPGCombatInterface>(Spec.GetContext().GetSourceObject());
    const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

    return 80.0f + 2.5f * Vigor + 10.0f * PlayerLevel;
}