#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AttributesComponent.h"
#include "BasePlayerCharacter.h"

AWeapon::AWeapon()
{
   
    CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
    CollisionCapsule->SetupAttachment(SceneRoot);
    CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
   
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    // Znajdź Capsule Component wśród komponentów aktora
   // CollisionCapsule = FindComponentByClass<UCapsuleComponent>();

    if (CollisionCapsule)
    {
        // Podepnij funkcję do zdarzenia kolizji
        CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnCollisionCapsuleBeginOverlap);

        // Upewnij się, że kolizja jest początkowo wyłączona
      

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

    // Unikaj trafienia samego siebie i właściciela
    if (OtherActor == GetOwner() || OtherActor == this || AlreadyHitActors.Contains(OtherActor))
        return;

    AlreadyHitActors.Add(OtherActor);

    // Sprawdź czy aktor implementuje CombatInterface
    if (OtherActor->Implements<UCombatInterface>())
    {
        // Wywołaj GetHit na trafionym akrorze
        ICombatInterface::Execute_GetHit(OtherActor, SweepResult.Location);

        // Zadaj obrażenia przez AttributesComponent jeśli istnieje
        UAttributesComponent* Attributes = OtherActor->FindComponentByClass<UAttributesComponent>();
        if (Attributes && Attributes->IsAlive())
        {
            Attributes->ApplyDamage(Damage);
            UE_LOG(LogTemp, Warning, TEXT("Applied %f damage to %s"), Damage, *OtherActor->GetName());

            // Jeśli cel nie żyje po obrażeniach
            if (!Attributes->IsAlive())
            {
                UE_LOG(LogTemp, Warning, TEXT("%s has been defeated!"), *OtherActor->GetName());
            }
        }
    }

    // Dla celów debugowania - pokaż punkt trafienia
    UE_LOG(LogTemp, Warning, TEXT("Weapon hit: %s at location: %s"),
        *OtherActor->GetName(), *SweepResult.Location.ToString());
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
      //  FVector CurrentScale = MeshComp->GetComponentScale();
        // Odłącz
        //MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

        // Ręcznie ustaw transformację
       // FTransform SocketTransform = InParent->GetSocketTransform(InSocketName);
        //SocketTransform.SetRotation(FQuat::Identity); // Resetuj rotację

        //MeshComp->SetWorldTransform(SocketTransform);

        // Attach z KeepWorld
        FAttachmentTransformRules AttachmentRules(
            EAttachmentRule::SnapToTarget,
            EAttachmentRule::SnapToTarget,
            EAttachmentRule::KeepWorld,
            false
        );

       // MeshComp->AttachToComponent(InParent, AttachmentRules, InSocketName);
        SceneRoot->AttachToComponent(InParent,AttachmentRules,InSocketName);

      //  AttachToComponent(InParent,AttachmentRules,InSocketName);
      //  MeshComp->SetWorldScale3D(CurrentScale);

        UE_LOG(LogTemp, Warning, TEXT("MeshComp directly attached with reset rotation"));
    }
}


void AWeapon::PickUp_Implementation(ABasePlayerCharacter* ByCharacter)
{
   // SetActorEnableCollision(false);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
