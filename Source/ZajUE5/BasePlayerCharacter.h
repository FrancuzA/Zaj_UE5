// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BasePlayerCharacter.generated.h"


class AWeapon;
/**
 * 
 */
UCLASS()
class ZAJUE5_API ABasePlayerCharacter : public ABaseCharacter
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
