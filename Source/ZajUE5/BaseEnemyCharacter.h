#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CombatInterface.h"
#include "AttributesComponent.h"
#include "PawnState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemyCharacter.generated.h"

UCLASS()
class ZAJUE5_API ABaseEnemyCharacter : public ABaseCharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributesComponent* Attributes;

	virtual void GetHit_Implementation(FVector HitLocation);

	// Funkcje ataku
	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	bool CanAttack() const;

	// Audio
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* HitSound;

	// Animacja
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* HitMontage;

	UFUNCTION()
	void OnEnemyDeath();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetPatrolPoints(const TArray<FVector>& Points);

	UFUNCTION(BlueprintCallable, Category = "AI")
	FVector GetNextPatrolPoint();


	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	float GetAttackRange() const { return AttackRange; }

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	bool IsInAttackRange() const;



protected:
	virtual void BeginPlay() override;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 200.0f;

	void TryAttack();
	void ResetCombat();

	// Dodaj funkcję OnDeath, która będzie wywołana przez delegat
	UFUNCTION()
	void OnDeath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 CurrentPatrolIndex = 0;

	// Reference to AI Controller
	UPROPERTY()
	class AEnemyAIController* EnemyAIController;
};