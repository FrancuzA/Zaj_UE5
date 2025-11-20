#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Item.generated.h"

class UStaticMeshComponent;

UCLASS()
class ZAJUE5_API AItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AItem();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	// Implementacja interfejsu
	virtual void Interact_Implementation(AActor* Interactor) override;

	// Funkcja do podnoszenia
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Item")
	void PickUp(ABasePlayerCharacter* ByCharacter);
	virtual void PickUp_Implementation(ABasePlayerCharacter* ByCharacter);
};