#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UCLASS()
class ZAJUE5_API AWeapon : public AItem
{
    GENERATED_BODY()

public:
    AWeapon();

    // Override interakcji
    virtual void Interact_Implementation(AActor* Interactor) override;

    // Funkcja do wyposażania broni
    UFUNCTION(BlueprintCallable)
    void Equip(ABasePlayerCharacter* Equipper);

    // POPRAWIONA: Funkcja do podpięcia do komponentu z regułami transformacji
    UFUNCTION(BlueprintCallable)
    void AttachToSocket(USceneComponent* InParent, const FName& InSocketName);

    // Funkcja ataku
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Attack();
    virtual void Attack_Implementation();

    // Socket dla broni
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName WeaponSocketName = FName("WeaponSocket");
};