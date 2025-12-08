// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class ZAJUE5_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

    virtual void OnPossess(APawn* InPawn) override;
    virtual void BeginPlay() override;

    // Blackboard key names
    static const FName TargetActorKey;
    static const FName LastKnownLocationKey;
    static const FName PatrolPointKey;
    static const FName PawnStateKey;
    static const FName IsDeadKey;
    static const FName CanSeePlayerKey;

    // Combat functions
    UFUNCTION(BlueprintCallable, Category = "AI Combat")
    bool CanAttack() const;

    UFUNCTION(BlueprintCallable, Category = "AI Combat")
    void StartAttack();

    UFUNCTION(BlueprintCallable, Category = "AI Combat")
    void StopAttack();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UBlackboardComponent* BlackboardComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UBehaviorTreeComponent* BehaviorTreeComponent;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTree;


    // Perception
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComponent;

    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    UFUNCTION()
    void OnPawnDeath();

private:
    void SetupPerception();
    AActor* GetPlayerActor() const;
};
