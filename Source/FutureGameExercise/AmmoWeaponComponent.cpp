#include "AmmoWeaponComponent.h"

#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "FutureGameExerciseCharacter.h"
#include "HelpingTools.h"

UAmmoWeaponComponent::UAmmoWeaponComponent()
{
	MaxAmmo = 10;
	CurrentAmmo = MaxAmmo;
}

void UAmmoWeaponComponent::Reload()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	int ammoNeeded = MaxAmmo - CurrentAmmo;
	CurrentAmmo += Character->TakeOutAmmo(ammoNeeded);

	OnAmmoChange.Broadcast();

	Help::DisplayDebugMessage(TEXT("Reload happened"));

	//TODO Sound of reload
}

const int& UAmmoWeaponComponent::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

void UAmmoWeaponComponent::Fire()
{
	if (CurrentAmmo == 0)
	{
		//TODO Sound "Out of Ammo"
		return;
	}

	//It's dangerous
	UTP_WeaponComponent::Fire();
	
	--CurrentAmmo;

	OnAmmoChange.Broadcast();

	Help::DisplayDebugMessage(TEXT("Ammo left: %d "), CurrentAmmo);
}

void UAmmoWeaponComponent::AttachWeapon(AFutureGameExerciseCharacter* TargetCharacter)
{
	//It's also dangerous
	UTP_WeaponComponent::AttachWeapon(TargetCharacter);
	
	if (Character != nullptr || !Character->GetHasRifle())
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UAmmoWeaponComponent::Reload);
			}
		}
	}
}
