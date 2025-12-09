#include "BaseEnemyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayerCharacter.h"
#include "EnemyAIController.h"
#include "TimerManager.h" // DODAJ ten include
#include "Weapon.h"

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<AEnemyAIController>(GetController());

	// Rozpocznij sprawdzanie ataku
	if (Attributes)
	{
		Attributes->OnDeath.AddDynamic(this, &ABaseEnemyCharacter::OnDeath);
	}
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(CurrentWeaponClass,GetActorLocation(),GetActorRotation());
	//CurrentWeapon->Equip(this);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseEnemyCharacter::TryAttack, 1.0f, true);
}

void ABaseEnemyCharacter::Attack()
{
	if (!CanAttack()) return;

	SetCurrentState(EPawnState::InCombat);

	// Tutaj implementacja ataku przeciwnika
	UE_LOG(LogTemp, Warning, TEXT("Enemy attacking!"));
	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	// Zaplanuj następny atak
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseEnemyCharacter::TryAttack, AttackCooldown, false);
}

bool ABaseEnemyCharacter::CanAttack() const
{
	if (GetCurrentState() == EPawnState::Dead || GetCurrentState() == EPawnState::Hit || GetCurrentState() == EPawnState::Exhausted)
		return false;

	if (!Attributes || !Attributes->CanPayStaminaCost(20.0f)) // Use Attack cost
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

	// Update blackboard
	if (EnemyAIController && EnemyAIController->BlackboardComponent)
	{
		EnemyAIController->BlackboardComponent->SetValueAsEnum(
			EnemyAIController->PawnStateKey,
			(uint8)GetCurrentState()
		);
	}

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

	// Update blackboard
	if (EnemyAIController && EnemyAIController->BlackboardComponent)
	{
		EnemyAIController->BlackboardComponent->SetValueAsBool(
			EnemyAIController->IsDeadKey,
			true
		);
	}

	// Disable collision and movement
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
}

void ABaseEnemyCharacter::SetPatrolPoints(const TArray<FVector>& Points)
{
	PatrolPoints = Points;
}

FVector ABaseEnemyCharacter::GetNextPatrolPoint()
{
	if (PatrolPoints.Num() == 0) return GetActorLocation();

	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
	return PatrolPoints[CurrentPatrolIndex];
}

bool ABaseEnemyCharacter::IsInAttackRange() const
{
	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!Player) return false;

	float Distance = FVector::Distance(GetActorLocation(), Player->GetActorLocation());
	return Distance <= AttackRange;
}

void ABaseEnemyCharacter::Equip(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip) return;

	CurrentWeapon = WeaponToEquip;
	CurrentWeapon->SetOwner(this);

	// Debug: sprawdź czy socket istnieje
	if (GetMesh()->DoesSocketExist(WeaponSocketName))
	{
		// Sprawdź transformację socketu
		FTransform SocketTransform = GetMesh()->GetSocketTransform(WeaponSocketName);
		AttachToSocket(GetMesh(), WeaponSocketName);

	}
	else
	{
		// Wypisz wszystkie dostępne sockety
		TArray<FName> SocketNames = GetMesh()->GetAllSocketNames();
		UE_LOG(LogTemp, Warning, TEXT("Available sockets:"));
		return;
	}

	CurrentWeapon = WeaponToEquip;
	CurrentWeapon->SetOwner(this);
}

void ABaseEnemyCharacter::AttachToSocket(USceneComponent* InParent, const FName& InSocketName)
{
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false
		);

		CurrentWeapon->WeaponRoot->AttachToComponent(InParent, AttachmentRules, InSocketName);

		UE_LOG(LogTemp, Warning, TEXT("MeshComp directly attached with reset rotation"));
	
}
