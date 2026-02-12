// Copyright K.Taukach

#include "AI/TDRPGAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ATDRPGAIController::ATDRPGAIController()
{
    Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
    check(Blackboard);

    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
    check(BehaviorTreeComponent);
}