// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "BasePlayerCharacter.generated.h"


class AWeapon;
class UCameraComponent;
class USpringArmComponent;
class UInteractionComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ZAJUE5_API ABasePlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	UInteractionComponent* GetInteractionComponent() { return InteractionComponent; }

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetCameraLocation();
	FVector GetCameraForwardVector();

	// Equip function declared so Weapon::PickUp can call it
	UFUNCTION()
	void Equip(AWeapon* WeaponToEquip);
	void Interact();
	virtual void Attack();

protected:
	ABasePlayerCharacter();

	/** Input mapping context for this player */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* MappingContext;

	/** Input actions */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* ViewCamera;

	
	UPROPERTY()
	AWeapon* CurrentWeapon = nullptr;

	/** Socket name to attach weapon to */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocketName = NAME_None;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	
	virtual void BeginPlay() override;
	
};
