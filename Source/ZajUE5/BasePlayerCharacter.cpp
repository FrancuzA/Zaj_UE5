#include "BasePlayerCharacter.h"
#include "Weapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InteractionInterface.h"
#include "InteractionComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BasePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{
    // Utwórz komponent interakcji
    InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

    GetCharacterMovement()->bOrientRotationToMovement = false;
    bUseControllerRotationPitch = true;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 150.f;
    CameraBoom->bUsePawnControlRotation = true;
    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    ViewCamera->bUsePawnControlRotation = false;
    
    Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
}


void ABasePlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (MappingContext)
            {
                Subsystem->AddMappingContext(MappingContext, 0);
            }
        }
    }

    if (Attributes)
    {
        Attributes->OnHealthChanged.AddDynamic(this, &ABasePlayerCharacter::OnHealthChanged);
        Attributes->OnStaminaChanged.AddDynamic(this, &ABasePlayerCharacter::OnStaminaChanged);
        Attributes->OnDeath.AddDynamic(this, &ABasePlayerCharacter::OnDeath);
    }
}

void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::Move);
        EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::Look);
        EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::Interact);
        EIC->BindAction(AttackAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::Attack);
        EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
    }
}

void ABasePlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D AxisValue = Value.Get<FVector2D>();
    if (Controller)
    {
        AddControllerYawInput(AxisValue.X);
        AddControllerPitchInput(-AxisValue.Y);
    }
}

FVector ABasePlayerCharacter::GetCameraLocation()
{
    return ViewCamera ? ViewCamera->GetComponentLocation() : FVector::ZeroVector;
}

FVector ABasePlayerCharacter::GetCameraForwardVector()
{
    return ViewCamera ? ViewCamera->GetForwardVector() : FVector::ForwardVector;
}

void ABasePlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D moveValue = Value.Get<FVector2D>();

    if (Controller)
    {

        if (moveValue.X != 0.f) 
        {
            const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            AddMovementInput(Direction, moveValue.X);
        }

        if (moveValue.Y != 0.f)
        {
            const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            AddMovementInput(Direction, moveValue.Y);
        }
    }
}

void ABasePlayerCharacter::Jump()
{
    if (!CanJump())
    {
        ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
        if (PC)
        {
            PC->ShowStaminaWarning();
        }
        return;
    }

    // Pay stamina cost
    Attributes->PayStamina(StaminaCost_Jump);

    // Perform jump
    Super::Jump();
}

void ABasePlayerCharacter::StartSprint()
{
    if (Attributes && Attributes->CanPayStaminaCost(StaminaCost_Sprint) && GetCurrentState() != EPawnState::Hit)
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed *= 1.5f; // Increase speed
    }
}

void ABasePlayerCharacter::StopSprint()
{
    if (bIsSprinting)
    {
        bIsSprinting = false;
        GetCharacterMovement()->MaxWalkSpeed /= 1.5f; // Reset speed
    }
}

void ABasePlayerCharacter::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
    if (PC)
    {
        PC->UpdateHealthBar(CurrentHealth, MaxHealth);
    }

    // Update state if health is critical
    if (CurrentHealth <= 0.0f)
    {
        SetCurrentState(EPawnState::Dead);
    }
}

void ABasePlayerCharacter::OnStaminaChanged(float CurrentStamina, float MaxStamina)
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
    if (PC)
    {
        PC->UpdateStaminaBar(CurrentStamina, MaxStamina);
    }

    // Update state if stamina is exhausted
    if (CurrentStamina <= 0.0f && GetCurrentState() != EPawnState::Dead)
    {
        SetCurrentState(EPawnState::Exhausted);
    }
    else if (CurrentStamina > 0.0f && GetCurrentState() == EPawnState::Exhausted)
    {
        SetCurrentState(EPawnState::Idle);
    }
}

void ABasePlayerCharacter::OnDeath()
{
    SetCurrentState(EPawnState::Dead);

    // Disable input and movement
    DisableInput(nullptr);
    GetCharacterMovement()->DisableMovement();

    ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
    if (PC)
    {
        PC->UpdatePawnStateDisplay(EPawnState::Dead);
    }
}

void ABasePlayerCharacter::Interact()
{
    
        //UE_LOG(LogTemp, Warning, TEXT("KLIKNIĘTO E"));
        InteractionComponent->TryInteract();
   
}

void ABasePlayerCharacter::Attack()
{
    if (!CanAttack())
    {
        // Show stamina warning
        ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
        if (PC)
        {
            PC->ShowStaminaWarning();
        }
        return;
    }

    Attributes->PayStamina(StaminaCost_Attack);

    if (CurrentWeapon && AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
    {
        UE_LOG(LogTemp, Warning, TEXT("Wykonano atak"));
        GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
        SetCurrentState(EPawnState::InCombat);
    }
}

void ABasePlayerCharacter::EnableWeaponCollision()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->EnableWeaponCollision();
    }
}

void ABasePlayerCharacter::DisableWeaponCollision()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->DisableWeaponCollision();
    }
}

void ABasePlayerCharacter::Equip(AWeapon* WeaponToEquip)
{
    if (!WeaponToEquip) return;

    // Debug: sprawdź czy socket istnieje
    if (GetMesh()->DoesSocketExist(WeaponSocketName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Socket %s exists on mesh!"), *WeaponSocketName.ToString());

        // Sprawdź transformację socketu
        FTransform SocketTransform = GetMesh()->GetSocketTransform(WeaponSocketName);
        UE_LOG(LogTemp, Warning, TEXT("Socket transform - Location: %s, Rotation: %s"),
            *SocketTransform.GetLocation().ToString(),
            *SocketTransform.GetRotation().Rotator().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Socket %s NOT FOUND on mesh!"), *WeaponSocketName.ToString());

        // Wypisz wszystkie dostępne sockety
        TArray<FName> SocketNames = GetMesh()->GetAllSocketNames();
        UE_LOG(LogTemp, Warning, TEXT("Available sockets:"));
        for (const FName& SocketName : SocketNames)
        {
            UE_LOG(LogTemp, Warning, TEXT(" - %s"), *SocketName.ToString());
        }
        return;
    }

    CurrentWeapon = WeaponToEquip;
    CurrentWeapon->SetOwner(this);

    if (GetMesh() && CurrentWeapon)
    {
        // Debug przed attach
        UE_LOG(LogTemp, Warning, TEXT("Before attach - Weapon location: %s"),
            *CurrentWeapon->GetActorLocation().ToString());

        CurrentWeapon->MeshComp->SetSimulatePhysics(false);
        CurrentWeapon->MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        CurrentWeapon->AttachToSocket(GetMesh(), WeaponSocketName);

        CurrentWeapon->GetRootComponent()->SetWorldScale3D(FVector(0.15f));
        // Debug po attach
        UE_LOG(LogTemp, Warning, TEXT("After attach - Weapon location: %s"),
            *CurrentWeapon->GetActorLocation().ToString());
    }
}

void ABasePlayerCharacter::GetHit_Implementation(FVector HitLocation)
{
    // Implementacja reakcji gracza na trafienie
    UE_LOG(LogTemp, Warning, TEXT("Player hit at location: %s"), *HitLocation.ToString());

    if (Attributes)
    {
        Attributes->ApplyDamage(10.0f); // Przykładowe obrażenia
    }
}

bool ABasePlayerCharacter::CanAttack() const
{
    return Attributes &&
        Attributes->CanPayStaminaCost(StaminaCost_Attack) &&
        GetCurrentState() != EPawnState::Hit &&
        GetCurrentState() != EPawnState::Dead;
}

bool ABasePlayerCharacter::CanJump() const
{
    // Najpierw sprawdź oryginalne warunki
    bool bCanJump = Super::CanJump();

    // Potem dodaj nasze warunki staminy i stanu
    return bCanJump &&
        Attributes &&
        Attributes->CanPayStaminaCost(StaminaCost_Jump) &&
        GetCurrentState() != EPawnState::Hit &&
        GetCurrentState() != EPawnState::Dead;
}