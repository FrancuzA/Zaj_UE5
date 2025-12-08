#include "BTTask_FindPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "GameFramework/Pawn.h"

UBTTask_FindPatrolPoint::UBTTask_FindPatrolPoint()
{
    NodeName = TEXT("Find Patrol Point");
}

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    // Generate random point around pawn
    float RandomAngle = FMath::FRand() * 2 * PI;
    float RandomDistance = FMath::FRandRange(MinPatrolDistance, MaxPatrolDistance);

    FVector RandomPoint = Pawn->GetActorLocation() +
        FVector(FMath::Cos(RandomAngle) * RandomDistance,
            FMath::Sin(RandomAngle) * RandomDistance,
            0.0f);

    // Ensure point is not too low (ground level)
    RandomPoint.Z = Pawn->GetActorLocation().Z;

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(
        AEnemyAIController::PatrolPointKey,
        RandomPoint
    );

    UE_LOG(LogTemp, Warning, TEXT("Found patrol point at: %s"), *RandomPoint.ToString());

    return EBTNodeResult::Succeeded;
}