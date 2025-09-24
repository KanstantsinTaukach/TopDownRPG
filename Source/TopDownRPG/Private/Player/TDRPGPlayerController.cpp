// Copyright K.Taukach

#include "Player/TDRPGPlayerController.h"
#include "EnhancedInputSubsystems.h"

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