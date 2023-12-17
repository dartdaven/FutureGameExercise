#include "FutureGameExerciseCharacter.h"

#include "FutureGameExerciseProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

#include "AmmoCollectibleComponent.h"
#include "TP_WeaponComponent.h"
#include "HelpingTools.h"
#include "Grenade.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFutureGameExerciseCharacter::AFutureGameExerciseCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GrenadeSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GrenadeSpawnPoint"));
	GrenadeSpawnPoint->SetupAttachment(FirstPersonCameraComponent);
	GrenadeSpawnPoint->SetRelativeLocation(FVector(50.f, 0.f, 0.f));

	USkeletalMeshComponent* MainMesh = GetMesh();
	MainMesh->SetOnlyOwnerSee(true);
	MainMesh->SetupAttachment(FirstPersonCameraComponent);
	MainMesh->bCastDynamicShadow = false;
	MainMesh->CastShadow = false;
	MainMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	MainMesh->SetRelativeRotation(FirstPersonCameraComponent->GetRelativeRotation());

	MaxAmmoAmount = 30;
	AmmoAmount = 0;
}

//Is called from a Pawn class 
void AFutureGameExerciseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFutureGameExerciseCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFutureGameExerciseCharacter::Look);

		//Switch Weapon
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &AFutureGameExerciseCharacter::SwitchWeapon);

		EnhancedInputComponent->BindAction(ThrowGrenadeAction, ETriggerEvent::Triggered, this, &AFutureGameExerciseCharacter::ThrowGrenade);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFutureGameExerciseCharacter::SwitchWeapon()
{
	if (Weapons.Num() <= 1) return;

	int IndexOfActiveWeapon = Weapons.IndexOfByKey(ActiveWeapon);
	int IndexOfNewWeapon;

	if (IndexOfActiveWeapon != INDEX_NONE)
	{
		IndexOfNewWeapon = (IndexOfActiveWeapon == Weapons.Num() - 1) ? 0 : IndexOfActiveWeapon + 1;
	}
	else
	{
		Help::DisplayErrorMessage(TEXT("%s: Weapon not found in the array"), *GetNameSafe(this));
		return;
	}

	bHasRifle = false; //for animation purposes

	DeactivateWeapon(ActiveWeapon);
	OnWeaponSwitch.Broadcast();

	auto CallbackWithArguments = [this, IndexOfNewWeapon]() { ActivateWeapon(Weapons[IndexOfNewWeapon]); };

	FTimerHandle TimerHandle;

	//TODO get the right time from animation, so .5f won't be a magic number
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, CallbackWithArguments, .5f, false);
}

void AFutureGameExerciseCharacter::ActivateWeapon(UTP_WeaponComponent* WeaponToActivate)
{
	//TODO Check

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(WeaponToActivate->GetMappingContext(), 1);
		}
	}

	WeaponToActivate->SetVisibility(true, true);

	ActiveWeapon = WeaponToActivate;

	bHasRifle = true;
}

void AFutureGameExerciseCharacter::DeactivateWeapon(UTP_WeaponComponent* WeaponToDeactivate)
{
	//TODO Check

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(WeaponToDeactivate->GetMappingContext());
		}
	}

	WeaponToDeactivate->SetVisibility(false, true);
}

void AFutureGameExerciseCharacter::ThrowGrenade()
{
	if (GrenadeClass != nullptr || GetController() != nullptr)
	{

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FVector Location = GrenadeSpawnPoint->GetComponentLocation();
		FRotator Rotation = GrenadeSpawnPoint->GetComponentRotation();

		AGrenade* GrenadeInstance = GetWorld()->SpawnActor<AGrenade>(GrenadeClass, Location, Rotation, ActorSpawnParams);
		
		GrenadeInstance->SetOwner(this); //not usable right now but may be useful
	}
}

void AFutureGameExerciseCharacter::OnAmmoPickUp(UAmmoCollectibleComponent* AmmoComponent)
{
	int AmmoNeeded = MaxAmmoAmount - AmmoAmount;

	AmmoAmount += AmmoComponent->TryTakeAmmo(AmmoNeeded);

	OnAmmoChange.Broadcast();
}

void AFutureGameExerciseCharacter::OnWeaponPickUp(UTP_WeaponComponent* WeaponComponent)
{
	if (!IsValid(WeaponComponent))
	{
		Help::DisplayErrorMessage(TEXT("%s: Wrong pointer was passed as a weapon"), *GetNameSafe(this));
		return;
	}

	bool IsAlreadyObtained{ false };

	for (UTP_WeaponComponent* Weapon : Weapons)
	{
		if (GetNameSafe(WeaponComponent) == GetNameSafe(Weapon))
		{
			Help::DisplayDebugMessage(TEXT("%s: I already have this weapon"), *GetNameSafe(this));
			IsAlreadyObtained = true;
		}
	}

	if (!IsAlreadyObtained)
	{
		Weapons.Add(WeaponComponent);

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		WeaponComponent->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));

		WeaponComponent->SetCharacter(this);
		WeaponComponent->SetupWeapon();

		if (!bHasRifle) 
		{
			bHasRifle = true;
			ActivateWeapon(WeaponComponent);
		}
		else
		{
			DeactivateWeapon(WeaponComponent);
		}
	}
}

void AFutureGameExerciseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFutureGameExerciseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFutureGameExerciseCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AFutureGameExerciseCharacter::GetHasRifle()
{
	return bHasRifle;
}

int AFutureGameExerciseCharacter::TakeOutAmmo(const int& amountRequested)
{
	if (AmmoAmount >= amountRequested)
	{
		AmmoAmount -= amountRequested;
		return amountRequested;
	}
	else
	{
		int toReturn = AmmoAmount;
		AmmoAmount = 0;
		return toReturn;
	}

	OnAmmoChange.Broadcast();
}

const int& AFutureGameExerciseCharacter::GetAmmoAmount() const
{
	return AmmoAmount;
}
