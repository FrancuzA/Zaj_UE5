#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"      // interfejs
#include "InteractionComponent.h"      // komponent
#include "Item.generated.h"

UCLASS()
class ZAJUE5_API AItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AItem();

	// Implementacja interfejsu
	virtual void Interact(ABasePlayerCharacter* Interactor) override;

	// Wirtualna funkcja do podnoszenia – może być nadpisana
	virtual void PickUp(ABasePlayerCharacter* ByCharacter);

protected:
	// Komponent interakcji – oznacza, że przedmiot jest interaktywny
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInteractionComponent* InteractionComp;

	// Mesh przedmiotu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;
};