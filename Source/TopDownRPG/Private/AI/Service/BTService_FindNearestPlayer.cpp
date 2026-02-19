// Copyright K.Taukach


#include "AI/Service/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* OwningPawn = AIOwner->GetPawn();
    const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

    TArray<AActor*> ActorsWithTag;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);

    float ClosestDistance = TNumericLimits<float>::Max();
    AActor* ClosestActor = nullptr;
    for(AActor* ActorWithTag : ActorsWithTag)
    {
        if(IsValid(ActorWithTag) && IsValid(OwningPawn))
        {
            const float Distance = OwningPawn->GetDistanceTo(ActorWithTag);
            if(Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestActor = ActorWithTag;
            }
        }
    }

    UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
    UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}