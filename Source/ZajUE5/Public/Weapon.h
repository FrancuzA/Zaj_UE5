#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Item.h"
#include "Weapon.generated.h"

UCLASS()
class ZAJUE5_API AWeapon : public AItem
{
    GENERATED_BODY()

public:
    AWeapon();



    UFUNCTION(BlueprintCallable)
    void EnableWeaponCollision();

    UFUNCTION(BlueprintCallable)
    void DisableWeaponCollision();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* CollisionCapsule;

    // Override interakcji
    virtual void Interact_Implementation(AActor* Interactor) override;

    // Funkcja do wyposażania broni
    UFUNCTION(BlueprintCallable)
    void Equip(ABasePlayerCharacter* Equipper);

    // POPRAWIONA: Funkcja do podpięcia do komponentu z regułami transformacji
    UFUNCTION(BlueprintCallable)
    void AttachToSocket(USceneComponent* InParent, const FName& InSocketName);

    
    virtual void PickUp_Implementation(ABasePlayerCharacter* ByCharacter) override;

    // Socket dla broni
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName WeaponSocketName = FName("WeaponSocket");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float Damage = 20.0f;

protected:
    UFUNCTION()
    void OnCollisionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);


    TArray<AActor*> AlreadyHitActors;

    // Funkcja do znalezienia Capsule Component w BeginPlay
    virtual void BeginPlay() override;
};