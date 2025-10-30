#pragma once



#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UCLASS()
class ZAJUE5_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	// Nadpisana wersja PickUp – wywołuje Equip na postaci
	virtual void PickUp(ABasePlayerCharacter* ByCharacter) override;
};