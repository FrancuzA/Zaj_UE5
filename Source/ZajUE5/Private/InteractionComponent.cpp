// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::SphereTrace(FHitResult& SphereHit)
{
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors)
	{
		ActorToIgnore.AddUnique(Actor);
	}
	ABasePlayerCharacter* CppOwner = Cast<ABasePlayerCharacter>(GetOwner());

	FVector EndTrace = CppOwner->GetCameraLocation() + (CppOwner->GetCameraForwardVector() * 500.f);

	UKismetSystemLibrary::SphereTraceSingle(

		this,
		CppOwner->GetCameraLocation(),
		EndTrace,
		TraceSphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorToIgnore,
		EDrawDebugTrace::ForDuration,
		SphereHit,
		true
	);
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractionComponent::TryInteract()
{
	FHitResult SphereHit;
	SphereTrace(SphereHit);

	AActor* HitActor = SphereHit.GetActor();
	if (!HitActor) return;

	if (HitActor->Implements<UInteractionInterface>())
	{
		IInteractionInterface::Execute_Interact( HitActor, GetOwner());
		IgnoreActors.Add(HitActor);
	}
}

