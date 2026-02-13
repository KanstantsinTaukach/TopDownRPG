// Copyright K.Taukach


#include "AI/Service/BTService_FindNearestPlayer.h"
#include "AIController.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, *AIOwner->GetName());
    GEngine->AddOnScreenDebugMessage(2, 10.0f, FColor::Green, *ActorOwner->GetName());
}