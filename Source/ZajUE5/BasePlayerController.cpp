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
   
    ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
    if (PlayerCharacter)
    {
        PlayerCharacter->Interact(); 
    }
}

void ABasePlayerController::OnAttack()
{
    ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
    if (PlayerCharacter)
    {
       
        PlayerCharacter->Attack();
    }
}