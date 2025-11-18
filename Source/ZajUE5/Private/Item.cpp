#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "BasePlayerCharacter.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComp);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Interact_Implementation(AActor* Interactor)
{
	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(Interactor);
	if (Player)
	{
		PickUp(Player);
	}
}

void AItem::PickUp_Implementation(ABasePlayerCharacter* ByCharacter)
{
	// Podstawowa implementacja - ukryj przedmiot
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}