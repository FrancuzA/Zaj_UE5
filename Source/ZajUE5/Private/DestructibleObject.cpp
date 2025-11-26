#include "DestructibleObject.h"
#include "AttributesComponent.h"
#include "Components/StaticMeshComponent.h"

ADestructibleObject::ADestructibleObject()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
}

void ADestructibleObject::BeginPlay()
{
    Super::BeginPlay();
}

void ADestructibleObject::GetHit_Implementation(FVector HitLocation)
{
    HitCount++;

    if (HitCount >= 2)
    {
        Destroy();
    }
}