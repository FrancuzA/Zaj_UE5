#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"   // interfejs
#include "Item.generated.h"


class ABasePlayerCharacter;
class UInteractionComponent;
class UStaticMeshComponent;

UCLASS()
class ZAJUE5_API AItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AItem();

	// Implementacja interfejsu
	 void Interact_Implementation(ABasePlayerCharacter* Interactor);

	// Wirtualna funkcja do podnoszenia – może być nadpisana
	virtual void PickUp(ABasePlayerCharacter* ByCharacter);

	// Mesh przedmiotu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

protected:
	// Komponent interakcji – oznacza, że przedmiot jest interaktywny
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInteractionComponent* InteractionComp;

};