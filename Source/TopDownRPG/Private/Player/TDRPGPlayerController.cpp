// Copyright K.Taukach

#include "Player/TDRPGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

 ATDRPGPlayerController::ATDRPGPlayerController()
{
     bReplicates = true;     
}

void ATDRPGPlayerController::BeginPlay()
{
     Super::BeginPlay();
     
     check(PlayerMappingContext);

     UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
     check(Subsystem);
     Subsystem->AddMappingContext(PlayerMappingContext, 0);

     bShowMouseCursor = true;
     DefaultMouseCursor = EMouseCursor::Default;

     FInputModeGameAndUI InputModeData;
     InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
     InputModeData.SetHideCursorDuringCapture(false);
     SetInputMode(InputModeData);
}

void ATDRPGPlayerController::SetupInputComponent()
{
     Super::SetupInputComponent();

     UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
     EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATDRPGPlayerController::Move);
     
}

void ATDRPGPlayerController::Move(const FInputActionValue& InputActionValue)
{
     const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
     const FRotator Rotation = GetControlRotation();
     const FRotator YawRotation(0.0, Rotation.Yaw, 0.0);

     const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
     const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

     if(APawn* ControlledPawn = GetPawn<APawn>())
     {
         ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
         ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
     }
}