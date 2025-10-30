#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// Klasa reprezentująca interfejs w systemie Unreal (wymagana)
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{	
 GENERATED_BODY()
};

// Czysty interfejs – implementowany przez klasy aktorów
class ZAJUE5_API IInteractionInterface
{					
 GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
 void Interact(class ABasePlayerCharacter* Interactor);
};
