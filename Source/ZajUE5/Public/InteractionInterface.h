#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractionInterface : public UInterface
{
    GENERATED_BODY()
};

class ZAJUE5_API IInteractionInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Equip(AActor* Equipper);
};