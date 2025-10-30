// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "BasePlayerCharacter.h"
void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("IA_Interact", IE_Pressed, this, &ABasePlayerController::OnInteract);
	InputComponent->BindAction("IA_Attack", IE_Pressed, this, &ABasePlayerController::OnAttack);
}

void ABasePlayerController::OnInteract()
{
	if (ABasePlayerCharacter* PC = Cast<ABasePlayerCharacter>(GetPawn()))
	{
		PC->OnInteract();
	}
}

void ABasePlayerController::OnAttack()
{
	if (ABasePlayerCharacter* PC = Cast<ABasePlayerCharacter>(GetPawn()))
	{
		PC->OnAttack();
	}
}