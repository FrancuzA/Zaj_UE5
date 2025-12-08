#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPoint.generated.h"

UCLASS()
class ZAJUE5_API UBTTask_FindPatrolPoint : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_FindPatrolPoint();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, Category = "Patrol")
    float MinPatrolDistance = 200.0f;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float MaxPatrolDistance = 500.0f;
};