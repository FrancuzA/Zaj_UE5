#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"   // interfejs
#include "Item.generated.h"


class ABasePlayerCharacter;
class UInteractionComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class USphereComponent;
class UCapsuleComponent;

UCLASS()
class ZAJUE5_API AItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AItem();


	// Wirtualna funkcja do podnoszenia – może być nadpisana
	virtual void PickUp(ABasePlayerCharacter* ByCharacter);

	// Mesh przedmiotu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

protected:
	virtual void BeginPlay() override;
	virtual void Interact_Implementation(AActor* OuterActor);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ItemSkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComp; 
};