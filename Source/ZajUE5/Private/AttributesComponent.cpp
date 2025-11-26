#include "AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Health = MaxHealth;
    Stamina = MaxStamina;
}

void UAttributesComponent::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;
    Stamina = MaxStamina;
}

void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Regenerate stamina
    if (bIsRegeneratingStamina && Stamina < MaxStamina)
    {
        float NewStamina = Stamina + (StaminaCost.StaminaRegenRate * DeltaTime);
        SetStamina(FMath::Min(NewStamina, MaxStamina));
    }
}

void UAttributesComponent::SetHealth(float NewHealth)
{
    float OldHealth = Health;
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

    if (Health != OldHealth)
    {
        OnHealthChanged.Broadcast(Health, MaxHealth);
    }

    if (Health <= 0.0f)
    {
        OnDeath.Broadcast();
    }
}

void UAttributesComponent::ApplyDamage(float Damage)
{
    SetHealth(Health - Damage);
}

void UAttributesComponent::SetStamina(float NewStamina)
{
    float OldStamina = Stamina;
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

    if (Stamina != OldStamina)
    {
        OnStaminaChanged.Broadcast(Stamina, MaxStamina);
    }
}

void UAttributesComponent::PayStamina(float Cost)
{
    if (Cost > 0.0f)
    {
        SetStamina(Stamina - Cost);
        bIsRegeneratingStamina = false;

        // Restart regeneration after a short delay
        FTimerHandle RegenerationTimer;
        GetWorld()->GetTimerManager().SetTimer(RegenerationTimer, this, &UAttributesComponent::StartStaminaRegeneration, 1.0f, false);
    }
}