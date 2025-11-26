#include "MainHUD.h"
#include "BasePlayerController.h"

AMainHUD::AMainHUD()
{
}

void AMainHUD::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<ABasePlayerController>(GetOwningPlayerController());
}