#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BasePlayerCharacter.h"
#include "InteractionInterface.generated.h"


UINTERFACE()
class UInteractionInterface : public UInterface
{	
 GENERATED_BODY()
};

class ZAJUE5_API IInteractionInterface
{					
 GENERATED_BODY()

public:
  UFUNCTION(BlueprintNativeEvent)
  void Interact(AActor* OuterActor);
};
