#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatInterface.h"
#include "DestructibleObject.generated.h"

UCLASS()
class ZAJUE5_API ADestructibleObject : public AActor, public ICombatInterface
{
    GENERATED_BODY()

public:
    ADestructibleObject();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UAttributesComponent* Attributes;

    int32 HitCount = 0;

public:
    virtual void GetHit_Implementation(FVector HitLocation) override;
};