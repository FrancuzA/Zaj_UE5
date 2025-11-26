#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CombatInterface.h"
#include "AttributesComponent.h"
#include "PawnState.h"
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
};