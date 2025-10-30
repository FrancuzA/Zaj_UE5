#include "Item.h"
#include "InteractionComponent.h"      // komponent
#include "Components/StaticMeshComponent.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// Utwórz mesh i ustaw jako root
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComp);

	// Dodaj komponent interakcji
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComp"));
}

void AItem::Interact_Implementation(ABasePlayerCharacter* Interactor)
{

}

void AItem::PickUp(ABasePlayerCharacter* ByCharacter)
{
	// Domyślnie: zniszcz przedmiot po podniesieniu
	Destroy();
}