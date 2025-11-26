#include "BaseEnemyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayerCharacter.h"
#include "TimerManager.h" // DODAJ ten include

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Rozpocznij sprawdzanie ataku
	if (Attributes)
	{
		Attributes->OnDeath.AddDynamic(this, &ABaseEnemyCharacter::OnDeath);
	}

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseEnemyCharacter::TryAttack, 1.0f, true);
}

void ABaseEnemyCharacter::Attack()
{
	if (!CanAttack()) return;

	SetCurrentState(EPawnState::InCombat);

	// Tutaj implementacja ataku przeciwnika
	UE_LOG(LogTemp, Warning, TEXT("Enemy attacking!"));

	// Zaplanuj następny atak
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseEnemyCharacter::TryAttack, AttackCooldown, false);
}

bool ABaseEnemyCharacter::CanAttack() const
{
	if (GetCurrentState() != EPawnState::Idle && GetCurrentState() != EPawnState::InCombat)
		return false;

	// Sprawdź czy gracz jest w zasięgu
	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!Player) return false;

	float Distance = FVector::Distance(GetActorLocation(), Player->GetActorLocation());
	return Distance <= AttackRange;
}

void ABaseEnemyCharacter::TryAttack()
{
	if (CanAttack())
	{
		Attack();
	}
	else
	{
		SetCurrentState(EPawnState::Idle);
	}
}

void ABaseEnemyCharacter::ResetCombat()
{
	if (Attributes && Attributes->IsAlive())
	{
		SetCurrentState(EPawnState::Idle);
		TryAttack();
	}
	else
	{
		SetCurrentState(EPawnState::Dead);
	}
}

void ABaseEnemyCharacter::OnDeath()
{
	SetCurrentState(EPawnState::Dead);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ABaseEnemyCharacter::GetHit_Implementation(FVector HitLocation)
{
	if (GetCurrentState() == EPawnState::Dead) return;

	SetCurrentState(EPawnState::Hit);

	// Odtwórz dźwięk
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	// Odtwórz animację
	if (HitMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
	}

	// Przerwij atak
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	// Update attributes
	if (Attributes)
	{
		Attributes->ApplyDamage(10.0f); // Example damage
	}

	// Wróć do stanu Idle po animacji
	FTimerHandle ResetTimer;
	GetWorld()->GetTimerManager().SetTimer(ResetTimer, this, &ABaseEnemyCharacter::ResetCombat, 1.0f, false);
}

void ABaseEnemyCharacter::OnEnemyDeath()
{
	SetCurrentState(EPawnState::Dead);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	// Disable collision and physics
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);

	// Destroy after delay
	SetLifeSpan(5.0f);
}