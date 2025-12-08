// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BasePlayerCharacter.h"
#include "BaseEnemyCharacter.h"
#include "AttributesComponent.h"

const FName AEnemyAIController::TargetActorKey = TEXT("TargetActor");
const FName AEnemyAIController::LastKnownLocationKey = TEXT("LastKnownPlayerLocation");
const FName AEnemyAIController::PatrolPointKey = TEXT("NextPatrolPoint");
const FName AEnemyAIController::PawnStateKey = TEXT("PawnState");
const FName AEnemyAIController::IsDeadKey = TEXT("IsDead");
const FName AEnemyAIController::CanSeePlayerKey = TEXT("CanSeePlayer");

AEnemyAIController::AEnemyAIController()
{
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    // Setup sight perception
    SetupPerception();
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ABaseEnemyCharacter* EnemyCharacter = Cast<ABaseEnemyCharacter>(InPawn);
    if (EnemyCharacter && BehaviorTree)
    {
        // Initialize blackboard
        if (BlackboardComponent)
        {
            BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
        }

        // Set initial values
        BlackboardComponent->SetValueAsBool(IsDeadKey, false);
        BlackboardComponent->SetValueAsEnum(PawnStateKey, (uint8)EnemyCharacter->GetCurrentState());
        BlackboardComponent->SetValueAsBool(CanSeePlayerKey, false);

        // Start behavior tree
        if (BehaviorTreeComponent)
        {
            BehaviorTreeComponent->StartTree(*BehaviorTree);
        }

        // Bind to death event
        if (UAttributesComponent* Attributes = EnemyCharacter->Attributes)
        {
            Attributes->OnDeath.AddDynamic(this, &AEnemyAIController::OnPawnDeath);
        }
    }
}

void AEnemyAIController::SetupPerception()
{
    // Create sight config
    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 2000.0f;
        SightConfig->LoseSightRadius = 2200.0f;
        SightConfig->PeripheralVisionAngleDegrees = 60.0f;
        SightConfig->SetMaxAge(5.0f);
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        AIPerceptionComponent->ConfigureSense(*SightConfig);
        AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
        AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
    }
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(Actor);
    if (!Player) return;

    if (BlackboardComponent)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            // Player detected
            BlackboardComponent->SetValueAsObject(TargetActorKey, Player);
            BlackboardComponent->SetValueAsVector(LastKnownLocationKey, Player->GetActorLocation());
            BlackboardComponent->SetValueAsBool(CanSeePlayerKey, true);
        }
        else
        {
            // Player lost
            BlackboardComponent->SetValueAsVector(LastKnownLocationKey, Player->GetActorLocation());
            BlackboardComponent->ClearValue(TargetActorKey);
            BlackboardComponent->SetValueAsBool(CanSeePlayerKey, false);
        }
    }
}

void AEnemyAIController::OnPawnDeath()
{
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool(IsDeadKey, true);
    }
}

bool AEnemyAIController::CanAttack() const
{
    ABaseEnemyCharacter* EnemyCharacter = Cast<ABaseEnemyCharacter>(GetPawn());
    if (!EnemyCharacter) return false;

    return EnemyCharacter->CanAttack();
}

void AEnemyAIController::StartAttack()
{
    ABaseEnemyCharacter* EnemyCharacter = Cast<ABaseEnemyCharacter>(GetPawn());
    if (EnemyCharacter)
    {
        EnemyCharacter->Attack();
    }
}

void AEnemyAIController::StopAttack()
{
    // Implement if needed
}

AActor* AEnemyAIController::GetPlayerActor() const
{
    return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}