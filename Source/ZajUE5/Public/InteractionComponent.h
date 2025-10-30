#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZAJUE5_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Tekst wyświetlany przy interakcji (np. w UI)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionText = FText::FromString("Interact");

	// Maksymalna odległość interakcji (cm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange = 200.0f;

	// Maksymalny kąt od osi wzroku (0 = tylko prosto przed sobą)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionAngle = 45.0f;
};