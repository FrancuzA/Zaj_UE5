#pragma once
#include "Weapon.generated.h"
#include "CoreMinimal.h"
#include "Item.h"


UCLASS()
class ZAJUE5_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	// Nadpisana wersja PickUp – wywołuje Equip na postaci
	virtual void PickUp(ABasePlayerCharacter* ByCharacter) override;
};