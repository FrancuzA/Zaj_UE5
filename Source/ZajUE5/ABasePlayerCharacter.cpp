// Fill out your copyright notice in the Description page of Project Settings.


#include "ABasePlayerCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"

void ABasePlayerCharacter::OnInteract()
{
	AActor* Target = FindInteractable();
	if (Target && Target->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		IInteractionInterface::Execute_Interact(Target, this);
	}
}

void ABasePlayerCharacter::OnAttack()
{
	if (CurrentWeapon)
	{
		UseWeapon(); // Wywołuje event w Blueprint
	}
}

void ABasePlayerCharacter::Equip(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}

	CurrentWeapon = WeaponToEquip;
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentWeapon->SetActorEnableCollision(false);

	if (GetMesh() && CurrentWeapon->MeshComp)
	{
		CurrentWeapon->MeshComp->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocketName
		);
	}
}

AActor* ABasePlayerCharacter::FindInteractable() const
{
	UCameraComponent* Cam = Cast<UCameraComponent>(GetFollowCamera());
	if (!Cam) return nullptr;

	FVector Start = Cam->GetComponentLocation();
	FVector End = Start + Cam->GetForwardVector() * 200.0f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			return HitActor;
		}
	}
	return nullptr;
}