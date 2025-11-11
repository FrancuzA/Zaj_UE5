#include "Weapon.h"
#include "BaseCharacter.h" 


void AWeapon::Interact_Implementation(AActor* OuterActor) 
{
	Super::Interact_Implementation(OuterActor);
	Equip(OuterActor);
}

void AWeapon::Equip(AActor* OuterActor)
{
	ABaseCharacter* BaseChar = Cast<ABaseCharacter>(OuterActor);
	if (BaseChar)
	{
		AttachToSocket(BaseChar->GetMesh(), MainSocketName);
		BaseChar->SetCurrentWeapon(this);
	}
}

void AWeapon:: AttachToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	TransformRules.ScaleRule = EAttachmentRule::KeepWorld;
	RootComponent->AttachToComponent(InParent, TransformRules, InSocketName);
}