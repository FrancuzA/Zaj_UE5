#include "Weapon.h"
#include "BasePlayerCharacter.h" // Musi istnieć!

void AWeapon::PickUp(ABasePlayerCharacter* ByCharacter)
{
	if (ByCharacter)
	{
		ByCharacter->Equip(this);
		// Nie niszczymy tu – broń zostanie przeniesiona do socketa
	}
}