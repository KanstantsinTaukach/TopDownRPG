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
#include "GameFramework/Character.h"
#include "UI/Widget/TDRPGDamageTextComponent.h"

ATDRPGPlayerController::ATDRPGPlayerController()
{
     bReplicates = true;
     Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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

void ATDRPGPlayerController::PlayerTick(float DeltaTime)
 {
     Super::PlayerTick(DeltaTime);

     CursorTrace();
     AutoRun();
 }

void ATDRPGPlayerController::AutoRun()
{
    if(!bAutoRunning) return;
    
    if(APawn* ControlledPawn = GetPawn())
    {
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
        const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
        ControlledPawn->AddMovementInput(Direction);

        const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
        if(DistanceToDestination <= AutoRunAcceptanceRadius)
        {
            bAutoRunning = false;
        }
    }
}

void ATDRPGPlayerController::CursorTrace()
{
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if(!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = Cast<ITDRPGEnemyInterface>(CursorHit.GetActor());

    if(LastActor != ThisActor)
    {
        if(LastActor) LastActor->UnHighlightActor();
        if(ThisActor) ThisActor->HighlightActor();
    }
}

void ATDRPGPlayerController::SetupInputComponent()
{
     Super::SetupInputComponent();

     UTDRPGInputComponent* TDRPGInputComponent = CastChecked<UTDRPGInputComponent>(InputComponent);
     TDRPGInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATDRPGPlayerController::Move);
     TDRPGInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ATDRPGPlayerController::ShiftPressed);
     TDRPGInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ATDRPGPlayerController::ShiftReleased);
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
        if(GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
        return;
    }

    if(GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
    
    if(!bTargeting && !bShiftKeyDown)
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
                }
                CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
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
        if(GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
        return;
    }

    if(bTargeting || bShiftKeyDown)
    {
        if(GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
    }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();
        
        if(CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

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

void ATDRPGPlayerController::ShowDamageNumber_Implementation(ACharacter* TargetCharacter, float DamageAmount)
{
    if(IsValid(TargetCharacter) && DamageTextComponentClass)
    {
        UTDRPGDamageTextComponent* DamageTextComponent = NewObject<UTDRPGDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
        DamageTextComponent->RegisterComponent();
        DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        DamageTextComponent->SetDamageText(DamageAmount);        
    }
}