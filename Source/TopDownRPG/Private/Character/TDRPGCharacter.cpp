// Copyright K.Taukach

#include "Character/TDRPGCharacter.h"
#include "AbilitySystemComponent.h"
#include "LevelUpInfo.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "UI/HUD/TDRPGHUD.h"
#include "Player/TDRPGPlayerController.h"
#include "Player/TDRPGPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ATDRPGCharacter::ATDRPGCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->SetUsingAbsoluteRotation(true);
    SpringArmComponent->bDoCollisionTest = false;
    
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;

    LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
    LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
    LevelUpNiagaraComponent->bAutoActivate = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0, 400.0, 0.0);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    CharacterClass = ECharacterClass::Elementalist;
}

void ATDRPGCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // Init ability actor info for the Server
    InitAbilityActorInfo();
    AddCharacterAbilities();
}

void ATDRPGCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    // Init ability actor info for the Client
    InitAbilityActorInfo();
}

void ATDRPGCharacter::InitAbilityActorInfo()
{
    ATDRPGPlayerState* TDRPGPlayerState = GetPlayerState<ATDRPGPlayerState>();
    check(TDRPGPlayerState);
    TDRPGPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TDRPGPlayerState, this);
    Cast<UTDRPGAbilitySystemComponent>(TDRPGPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
    
    AbilitySystemComponent = TDRPGPlayerState->GetAbilitySystemComponent();
    AttributeSet = TDRPGPlayerState->GetAttributeSet();
    
    if(ATDRPGPlayerController* TDRPGPlayerController = Cast<ATDRPGPlayerController>(GetController()))
    {        
        if(ATDRPGHUD* TDRPGHUD = Cast<ATDRPGHUD>(TDRPGPlayerController->GetHUD()))
        {
            TDRPGHUD->InitOverlay(TDRPGPlayerController, TDRPGPlayerState, AbilitySystemComponent, AttributeSet);
        }
    }

    InitializeDefaultAttributes();
}

int32 ATDRPGCharacter::GetPlayerLevel_Implementation() const
{
    const ATDRPGPlayerState* TDRPGPlayerState = GetPlayerState<ATDRPGPlayerState>();
    check(TDRPGPlayerState);
    return TDRPGPlayerState->GetPlayerLevel();
}

int32 ATDRPGCharacter::GetXP_Implementation() const
{
    const ATDRPGPlayerState* TDRPGPlayerState = GetPlayerState<ATDRPGPlayerState>();
    check(TDRPGPlayerState);
    return TDRPGPlayerState->GetXP();
}

void ATDRPGCharacter::AddToXP_Implementation(int32 InXP)
{
    ATDRPGPlayerState* TDRPGPlayerState = GetPlayerState<ATDRPGPlayerState>();
    check(TDRPGPlayerState);
    TDRPGPlayerState->AddToXP(InXP);
}

int32 ATDRPGCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
    const ATDRPGPlayerState* TDRPGPlayerState = GetPlayerState<ATDRPGPlayerState>();
    check(TDRPGPlayerState);
    return TDRPGPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

void ATDRPGCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
    //TODO: Add AttributePoints to PlayerState
}

int32 ATDRPGCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
    const ATDRPGPlayerState* TDRPGPlayerState = GetPlayerState<ATDRPGPlayerState>();
    check(TDRPGPlayerState);
    return TDRPGPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void ATDRPGCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
    //TODO: Add SpellPoints to PlayerState
}

void ATDRPGCharacter::LevelUp_Implementation()
{
    MulticastLevelUpParticles();
}

void ATDRPGCharacter::MulticastLevelUpParticles_Implementation() const
{
    if(IsValid(LevelUpNiagaraComponent))
    {
        const FVector CameraLocation = CameraComponent->GetComponentLocation();
        const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
        const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
        LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
        LevelUpNiagaraComponent->Activate(true);
    }
}

int32 ATDRPGCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
    const ATDRPGPlayerState* TDRPGPlayerState = GetPlayerState<ATDRPGPlayerState>();
    check(TDRPGPlayerState);
    return TDRPGPlayerState->LevelUpInfo->FindLevelForXP(InXP);    
}

void ATDRPGCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
    ATDRPGPlayerState* TDRPGPlayerState = GetPlayerState<ATDRPGPlayerState>();
    check(TDRPGPlayerState);
    return TDRPGPlayerState->AddToLevel(InPlayerLevel);
}