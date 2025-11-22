// Copyright K.Taukach

#include "Player/TDRPGPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "TDRPGAbilitySystemComponent.h"
#include "INteraction/TDRPGEnemyInterface.h"
#include "Input/TDRPGInputComponent.h"

ATDRPGPlayerController::ATDRPGPlayerController()
{
     bReplicates = true;     
}

void ATDRPGPlayerController::PlayerTick(float DeltaTime)
 {
     Super::PlayerTick(DeltaTime);

     CursorTrace();
 }

void ATDRPGPlayerController::BeginPlay()
{
     Super::BeginPlay();
     
     check(PlayerMappingContext);

     UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
     if(Subsystem)
     {
         Subsystem->AddMappingContext(PlayerMappingContext, 0);
     }     

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

     UTDRPGInputComponent* TDRPGInputComponent = CastChecked<UTDRPGInputComponent>(InputComponent);
     TDRPGInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATDRPGPlayerController::Move);

     TDRPGInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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

void ATDRPGPlayerController::CursorTrace()
{
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if(!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = Cast<ITDRPGEnemyInterface>(CursorHit.GetActor());

    if(LastActor == nullptr)
    {
        if(ThisActor != nullptr)
        {
            ThisActor->HighlightActor();
        }
    }
    else
    {
        if(ThisActor == nullptr)
        {
            LastActor->UnHighlightActor();
        }
        else
        {
            if(LastActor != ThisActor)
            {
                LastActor->UnHighlightActor();
                ThisActor->HighlightActor();
            }
        }
    }
}

void ATDRPGPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    //GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, *InputTag.ToString());
}

void ATDRPGPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if(GetASC() == nullptr) return;
    
    GetASC()->AbilityInputTagReleased(InputTag);
}

void ATDRPGPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if(GetASC() == nullptr) return;
    
    GetASC()->AbilityInputTagHeld(InputTag);
}

UTDRPGAbilitySystemComponent* ATDRPGPlayerController::GetASC()
{
    if(AbilitySystemComponent == nullptr)
    {
        AbilitySystemComponent = Cast<UTDRPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
    }

    return AbilitySystemComponent;
}
