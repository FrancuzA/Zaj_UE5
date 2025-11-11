#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasePlayerCharacter.h"
#include "InteractionInterface.h"
#include "InteractionComponent.generated.h"

class UKismetSystemLibrary;
UCLASS()
class ZAJUE5_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditDefaultsOnly)
	TArray<AActor*> IgnoreActors;

	UPROPERTY()
	float TraceSphereRadius = 10.f;

	void TryInteract();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SphereTrace(FHitResult& SphereHit);


};