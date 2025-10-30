// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZAJUE5_API AABasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;

private:
	void OnInteract();
	void OnAttack();
};
