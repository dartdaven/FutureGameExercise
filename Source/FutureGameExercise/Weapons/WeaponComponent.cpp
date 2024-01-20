#include "WeaponComponent.h"

#include "../MainCharacter.h"
#include "../Collectibles/AmmoProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "../Misc/HelpingTools.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UWeaponComponent::SetupWeapon(AMainCharacter* a_Character)
{
	SetCharacter(a_Character);
}

void UWeaponComponent::SetupActionBindings()
{
	if (!IsValid(Character))
	{
		Help::DisplayErrorMessage(TEXT("%s: Unable to setup bindings due to the character is absent"), *GetNameSafe(this));
		return;
	}

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(WeaponMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::Fire);
		}
	}
}

void UWeaponComponent::ClearActionBindings()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(WeaponMappingContext);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->ClearBindingsForObject(this);
		}
	}
}

void UWeaponComponent::Fire()
{
	FireImpl();
}

bool UWeaponComponent::FireImpl()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return false;
	}

	bool ProjectileFired{ false }, SFXPlayed{ false }, AnimationPlayed{ false };

	// Try and fire a projectile
	if (IsValid(ProjectileClass))
	{
		UWorld* const World = GetWorld();
		if (IsValid(World))
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AAmmoProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			ProjectileFired = true;
		}
	}
	
	// Try and play the sound if specified
	if (IsValid(FireSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());

		SFXPlayed = true;
	}
	
	// Try and play a firing animation if specified
	if (IsValid(FireAnimation))
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance))
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);

			AnimationPlayed = true;
		}
	}

	return ProjectileFired && SFXPlayed && AnimationPlayed;
}

AMainCharacter* UWeaponComponent::GetCharacter() const
{
	return Character;
}

void UWeaponComponent::SetCharacter(AMainCharacter* a_Character)
{
	if (!IsValid(a_Character))
	{
		Help::DisplayErrorMessage(TEXT("%s: Wrong pointer was passed to a weapon as a character"), *GetNameSafe(this));
		return;
	}

	Character = a_Character;
}

const UInputMappingContext* UWeaponComponent::GetMappingContext() const
{
	return WeaponMappingContext;
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(WeaponMappingContext);
		}
	}
}
