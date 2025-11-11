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

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Interact_Implementation(AActor* OuterActor)
{
	ItemMesh->SetSimulatePhysics(false);
	SetOwner(OuterActor);
}

void AItem::PickUp(ABasePlayerCharacter* ByCharacter)
{
	// Domyślnie: zniszcz przedmiot po podniesieniu
	Destroy();
}