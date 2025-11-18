#include "Weapon.h"
#include "BasePlayerCharacter.h"

AWeapon::AWeapon()
{
   
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

void AWeapon::Attack_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Weapon Attack!"));
}