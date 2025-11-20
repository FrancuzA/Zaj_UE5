#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayerCharacter.h"

AWeapon::AWeapon()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    CollisionCapsule = nullptr;
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    // Znajdź Capsule Component wśród komponentów aktora
    CollisionCapsule = FindComponentByClass<UCapsuleComponent>();

    if (CollisionCapsule)
    {
        // Podepnij funkcję do zdarzenia kolizji
        CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnCollisionCapsuleBeginOverlap);

        // Upewnij się, że kolizja jest początkowo wyłączona
        CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        UE_LOG(LogTemp, Warning, TEXT("Found Capsule Component: %s"), *CollisionCapsule->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Capsule Component found in Weapon!"));
    }
}

void AWeapon::EnableWeaponCollision()
{
    if (CollisionCapsule)
    {
        CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        AlreadyHitActors.Empty();
        UE_LOG(LogTemp, Warning, TEXT("Weapon collision ENABLED"));
    }
}

void AWeapon::DisableWeaponCollision()
{
    if (CollisionCapsule)
    {
        CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        UE_LOG(LogTemp, Warning, TEXT("Weapon collision DISABLED"));
    }
}

void AWeapon::OnCollisionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!CollisionCapsule) return;

    if (OtherActor == GetOwner() || OtherActor == this || AlreadyHitActors.Contains(OtherActor))
        return;

    AlreadyHitActors.Add(OtherActor);

    FVector Start = CollisionCapsule->GetComponentLocation();
    FVector End = Start;
    float Radius = CollisionCapsule->GetScaledCapsuleRadius();
    float HalfHeight = CollisionCapsule->GetScaledCapsuleHalfHeight();

    FHitResult HitResult;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());
    ActorsToIgnore.Add(this);

    // Użyj SphereTrace zamiast BoxTrace dla Capsule Component
    UKismetSystemLibrary::SphereTraceSingle(
        this,
        Start,
        End,
        Radius,
        UEngineTypes::ConvertToTraceType(ECC_Visibility),
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForDuration,
        HitResult,
        true
    );

    if (HitResult.bBlockingHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon hit: %s at location: %s"),
            *OtherActor->GetName(), *HitResult.Location.ToString());
    }
}

void AWeapon::Interact_Implementation(AActor* Interactor)
{
    Super::Interact_Implementation(Interactor);

    ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(Interactor);
    if (Player)
    {
        Equip(Player);
    }
}

void AWeapon::Equip(ABasePlayerCharacter* Equipper)
{
   /* ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(Equipper);*/
    
        AttachToSocket(Equipper->GetMesh(),WeaponSocketName);
        Equipper->Equip(this);
    
}

// POPRAWIONA IMPLEMENTACJA
void AWeapon::AttachToSocket(USceneComponent* InParent, const FName& InSocketName)
{
    if (InParent && MeshComp)
    {
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        AttachmentRules.ScaleRule = EAttachmentRule::KeepWorld;

        RootComponent->AttachToComponent(InParent, AttachmentRules, InSocketName);
    }
}


void AWeapon::PickUp_Implementation(ABasePlayerCharacter* ByCharacter)
{
    SetActorEnableCollision(false);
}
