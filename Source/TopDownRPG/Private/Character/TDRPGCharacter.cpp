// Copyright K.Taukach

#include "Character/TDRPGCharacter.h"

#include "AbilitySystemComponent.h"
#include "Player/TDRPGPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ATDRPGCharacter::ATDRPGCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(RootComponent);
    
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0, 400.0, 0.0);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
}

void ATDRPGCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // Init ability actor info for the Server
    InitAbilityActorInfo();
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
    
    AbilitySystemComponent = TDRPGPlayerState->GetAbilitySystemComponent();
    AttributeSet = TDRPGPlayerState->GetAttributeSet();
}