#include "AmmoWeaponComponent.h"

#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"

#include "AmmoWidget.h"
#include "FutureGameExerciseCharacter.h"
#include "HelpingTools.h"

UAmmoWeaponComponent::UAmmoWeaponComponent()
{
	MaxAmmo = 10;
	CurrentAmmo = MaxAmmo;

	AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoWidgetComponent"));
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

const int& UAmmoWeaponComponent::GetMaxAmmo() const
{
	return MaxAmmo;
}

void UAmmoWeaponComponent::Fire()
{
	if (CurrentAmmo == 0)
	{
		//TODO Sound "Out of Ammo"
		return;
	}

	//If the parent fired successfully
	if (UTP_WeaponComponent::FireImpl())
	{
		--CurrentAmmo;

		OnAmmoChange.Broadcast();

		Help::DisplayDebugMessage(TEXT("Ammo left: %d "), CurrentAmmo);
	}
	else
	{
		Help::DisplayErrorMessage(TEXT("Fire method of Weapon Component is broken"));
	}
}

void UAmmoWeaponComponent::SetupWeapon()
{
		SetupWidget();

		SetupActionBindings();
}

void UAmmoWeaponComponent::SetupActionBindings()
{
	//if setup of the parent is successfull
	if (UTP_WeaponComponent::SetupActionBindings())
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UAmmoWeaponComponent::Reload);
	}
	else
	{
		Help::DisplayErrorMessage(TEXT("Something's wrong with bindings"));
	}
}

void UAmmoWeaponComponent::SetupWidget()
{
	//MagicNumbers Alert
	AmmoWidgetComponent->SetRelativeLocationAndRotation(FVector(-10.f, 40.f, 20.f), FQuat(FRotator(0.f, 270.f, 0.f)));
	AmmoWidgetComponent->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));

	AmmoWidgetComponent->SetCastShadow(false);

	AmmoWidgetComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	AmmoWidgetComponent->SetWidgetClass(AmmoWidget);
	
	UAmmoWidget* AmmoWidgetPtr = Cast<UAmmoWidget>(AmmoWidgetComponent->GetUserWidgetObject());

	if (IsValid(AmmoWidgetPtr))
	{
		AmmoWidgetPtr->SetWeapon(this);
	}
	else
	{
		Help::DisplayErrorMessage(TEXT("Widget class is not set correctly in the Weapon Blueprint"));
	}

}
