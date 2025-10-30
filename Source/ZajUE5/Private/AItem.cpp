#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "InteractionComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// Utwórz mesh i ustaw jako root
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComp);

	// Dodaj komponent interakcji
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComp"));
}

void AItem::Interact(ABasePlayerCharacter* Interactor)
{
	if (Interactor)
	{
		PickUp(Interactor);
	}
}

void AItem::PickUp(ABasePlayerCharacter* ByCharacter)
{
	// Domyślnie: zniszcz przedmiot po podniesieniu
	Destroy();
}