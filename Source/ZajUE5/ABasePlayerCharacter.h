// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "ABasePlayerCharacter.generated.h"
#include "AWeapon.h"

/**
 * 
 */
UCLASS()
class ZAJUE5_API AABasePlayerCharacter : public AABaseCharacter
{
	GENERATED_BODY()
public:
	void OnInteract();
	void OnAttack();
	void Equip(AWeapon* WeaponToEquip);

	UFUNCTION(BlueprintImplementableEvent)
	void UseWeapon();

protected:
	UPROPERTY()
	AWeapon* CurrentWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocketName = TEXT("WeaponSocket");

private:
	AActor* FindInteractable() const;
	
};
