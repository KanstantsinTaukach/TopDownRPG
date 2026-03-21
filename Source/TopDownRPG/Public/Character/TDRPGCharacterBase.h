// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Interaction/TDRPGCombatInterface.h"
#include "GameFramework/Character.h"
#include "TDRPGCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;

UCLASS()
class TOPDOWNRPG_API ATDRPGCharacterBase : public ACharacter, public IAbilitySystemInterface, public ITDRPGCombatInterface
{
    GENERATED_BODY()

public:
    ATDRPGCharacterBase();
    
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
    UAttributeSet* GetAttributeSet() const { return AttributeSet; };

    /** Combat Interface */
    virtual  UAnimMontage* GetHitReactMontage_Implementation() override { return HitReactMontage; };
    virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override { return AttackMontages; };

    virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
    virtual void Die() override;        
    virtual bool IsDead_Implementation() const override;    
    virtual AActor* GetAvatar_Implementation() override;
    /** end Combat Interface */

    UFUNCTION(NetMulticast, Reliable)
    virtual void MulticastHandleDeath();
    
protected:
    virtual void BeginPlay() override;

    virtual void InitAbilityActorInfo();

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;

    UPROPERTY(EditAnywhere, Category = "Combat")
    FName WeaponTipSocketName;

    UPROPERTY(EditAnywhere, Category = "Combat")
    FName LeftHandSocketName;
    
    UPROPERTY(EditAnywhere, Category = "Combat")
    FName RightHandSocketName;
    
    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FTaggedMontage> AttackMontages;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
    TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
    TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
    TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

    void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

    virtual void InitializeDefaultAttributes() const;

    void AddCharacterAbilities();

    /* Dissolve Effects */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UMaterialInstance> MeshDissolveMaterialInstance;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

    void Dissolve();

    UFUNCTION(BlueprintImplementableEvent)
    void StartMeshDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);

    UFUNCTION(BlueprintImplementableEvent)
    void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);
    
private:
    UPROPERTY(EditAnywhere, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>>StartupAbilities;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<UAnimMontage> HitReactMontage;
    
    bool bDead = false;
};
