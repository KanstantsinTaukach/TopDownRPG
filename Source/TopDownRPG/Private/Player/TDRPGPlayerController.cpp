// Copyright K.Taukach

#include "Player/TDRPGPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AbilitySystem/TDRPGAbilitySystemComponent.h"
#include "TDRPGGameplayTags.h"
#include "Interaction/TDRPGEnemyInterface.h"
#include "Input/TDRPGInputComponent.h"
#include "Components/SplineComponent.h"

ATDRPGPlayerController::ATDRPGPlayerController()
{
     bReplicates = true;

     Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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
    if(InputTag.MatchesTagExact(FTDRPGGameplayTags::Get().InputTag_LMB))
    {
        bTargeting = ThisActor ? true : false;
        bAutoRunning = false;   
    }
}

void ATDRPGPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if(!InputTag.MatchesTagExact(FTDRPGGameplayTags::Get().InputTag_LMB))
    {
        if(GetASC())
        {
            GetASC()->AbilityInputTagReleased(InputTag);
        }
        return;
    }

    if(bTargeting)
    {
        if(GetASC())
        {
            GetASC()->AbilityInputTagReleased(InputTag);
        }
    }
    else
    {
        APawn* ControlledPawn = GetPawn<APawn>();
        if(FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                Spline->ClearSplinePoints();
                for(const FVector& PointLoc : NavPath->PathPoints)
                {
                    Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
                    DrawDebugSphere(GetWorld(), PointLoc, 8.0f, 8, FColor::Green, false, 5.0f);
                }
                bAutoRunning = true;
            }
        }
        
        FollowTime = 0.0f;
        bTargeting = false;
    }
}

void ATDRPGPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if(!InputTag.MatchesTagExact(FTDRPGGameplayTags::Get().InputTag_LMB))
    {
        if(GetASC())
        {
            GetASC()->AbilityInputTagHeld(InputTag);
        }
        return;
    }

    if(bTargeting)
    {
        if(GetASC())
        {
            GetASC()->AbilityInputTagHeld(InputTag);
        }
    }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();

        FHitResult HitResult;
        if(GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
        {
            CachedDestination = HitResult.ImpactPoint;
        }

        if(APawn* ControlledPawn = GetPawn())
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection);
        }
    }    
}

UTDRPGAbilitySystemComponent* ATDRPGPlayerController::GetASC()
{
    if(AbilitySystemComponent == nullptr)
    {
        AbilitySystemComponent = Cast<UTDRPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
    }

    return AbilitySystemComponent;
}
