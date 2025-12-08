// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackPlayer.h"
#include "EnemyAIController.h"
#include "BaseEnemyCharacter.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
    NodeName = TEXT("Attack Player");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;

    if (AIController->CanAttack())
    {
        AIController->StartAttack();
        bIsAttacking = true;
        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

void UBTTask_AttackPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return;

    ABaseEnemyCharacter* Enemy = Cast<ABaseEnemyCharacter>(AIController->GetPawn());
    if (Enemy && Enemy->GetCurrentState() != EPawnState::InCombat)
    {
        bIsAttacking = false;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}