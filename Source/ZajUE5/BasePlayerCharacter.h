#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Animation/AnimMontage.h"
#include "CombatInterface.h"
#include "AttributesComponent.h"
#include "BasePlayerCharacter.generated.h"

class AWeapon;
class UCameraComponent;
class USpringArmComponent;
class UInteractionComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(Blueprintable)
class ZAJUE5_API ABasePlayerCharacter : public ABaseCharacter
{
    GENERATED_BODY()

public:
    ABasePlayerCharacter();

    // DODAJ TE FUNKCJE:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable)
    FVector GetCameraLocation();

    UFUNCTION(BlueprintCallable)
    FVector GetCameraForwardVector();

    UFUNCTION(BlueprintCallable)
    void Equip(AWeapon* WeaponToEquip);

    UFUNCTION(BlueprintCallable)
    void Interact();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* AttackMontage;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EnableWeaponCollision();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void DisableWeaponCollision();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAttributesComponent* Attributes;

    virtual void GetHit_Implementation(FVector HitLocation);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float StaminaCost_Attack = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float StaminaCost_Jump = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float StaminaCost_Sprint = 5.0f;

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    bool CanAttack() const;

    bool CanJump() const;

    // Override functions
    virtual void Attack() override;
    virtual void Jump() override;

    // Sprint
    UFUNCTION(BlueprintCallable)
    void StartSprint();

    UFUNCTION(BlueprintCallable)
    void StopSprint();

protected:
    virtual void BeginPlay() override;

    /** Input mapping context for this player */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* MappingContext;

    /** Input actions */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* InteractAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* AttackAction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UInteractionComponent* InteractionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* ViewCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon = nullptr;

    /** Socket name to attach weapon to */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponSocketName = "WeaponSocket";

private:
    
    void Move(const FInputActionValue& value);
    void Look(const FInputActionValue& value);

    UFUNCTION()
    void OnHealthChanged(float CurrentHealth, float MaxHealth);

    UFUNCTION()
    void OnStaminaChanged(float CurrentStamina, float MaxStamina);

    UFUNCTION()
    void OnDeath();

    bool bIsSprinting = false;
};