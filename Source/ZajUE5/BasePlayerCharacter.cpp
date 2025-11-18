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

void ABasePlayerCharacter::Interact()
{
    
        UE_LOG(LogTemp, Warning, TEXT("KLIKNIĘTO E"));
        InteractionComponent->TryInteract();
   
}

void ABasePlayerCharacter::Attack()
{
    // Jeśli mamy broń, wywołaj jej atak
    if (CurrentWeapon)
    {
        CurrentWeapon->Attack();
    }
}

void ABasePlayerCharacter::Equip(AWeapon* WeaponToEquip)
{
    if (!WeaponToEquip) return;

    /*if (CurrentWeapon)
    {
        CurrentWeapon->Destroy();
    }*/

    CurrentWeapon = WeaponToEquip;

    CurrentWeapon->SetOwner(this);

    if (GetMesh() && CurrentWeapon)
    {
        
        CurrentWeapon->MeshComp->SetSimulatePhysics(false);
        CurrentWeapon->MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        /*CurrentWeapon->AttachToComponent(
            GetMesh(),
            FAttachmentTransformRules::SnapToTargetNotIncludingScale,
            WeaponSocketName
        );*/
    }
}