// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TDRPGAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
    GENERATED_BODY()

    FEffectProperties(){}
    
    FGameplayEffectContextHandle EffectContextHandle;

    UPROPERTY()
    UAbilitySystemComponent* SourceASC = nullptr;
    UPROPERTY()
    AActor* SourceAvatarActor = nullptr;
    UPROPERTY()
    AController* SourceController = nullptr;
    UPROPERTY()
    ACharacter* SourceCharacter = nullptr;

    UPROPERTY()
    UAbilitySystemComponent* TargetASC = nullptr;
    UPROPERTY()
    AActor* TargetAvatarActor = nullptr;
    UPROPERTY()
    AController* TargetController = nullptr;
    UPROPERTY()
    ACharacter* TargetCharacter = nullptr;    
};

UCLASS()
class TOPDOWNRPG_API UTDRPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
    UTDRPGAttributeSet();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    
    /*
     * Vital Attributes
     */
    
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UTDRPGAttributeSet, Health);
    
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UTDRPGAttributeSet, MaxHealth);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UTDRPGAttributeSet, Mana);
    
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(UTDRPGAttributeSet, MaxMana);

    /*
     * Primary Attributes
     */

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
    FGameplayAttributeData Strength;
    ATTRIBUTE_ACCESSORS(UTDRPGAttributeSet, Strength);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
    FGameplayAttributeData Intelligence;
    ATTRIBUTE_ACCESSORS(UTDRPGAttributeSet, Intelligence);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
    FGameplayAttributeData Resilience;
    ATTRIBUTE_ACCESSORS(UTDRPGAttributeSet, Resilience);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
    FGameplayAttributeData Vigor;
    ATTRIBUTE_ACCESSORS(UTDRPGAttributeSet, Vigor);
    
    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
    UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
    UFUNCTION()
    void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

    UFUNCTION()
    void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
    UFUNCTION()
    void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
    UFUNCTION()
    void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
    UFUNCTION()
    void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

private:
    void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
