// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TDRPGAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class TOPDOWNRPG_API ATDRPGAIController : public AAIController
{
	GENERATED_BODY()

public:
    ATDRPGAIController();

protected:
    UPROPERTY()
    TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;	
};
