#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BasePlayerController.h"
#include "MainHUD.generated.h"

UCLASS()
class ZAJUE5_API AMainHUD : public AHUD
{
    GENERATED_BODY()

public:
    AMainHUD();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateHealth(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateStamina(float CurrentStamina, float MaxStamina);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdatePawnState(EPawnState NewState);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void ShowStaminaWarning();

private:
    UPROPERTY()
    class ABasePlayerController* PlayerController;
};