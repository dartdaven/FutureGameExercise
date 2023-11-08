#include "HeatWeaponComponent.h"

#include "FutureGameExerciseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void UHeatWeaponComponent::AttachWeapon(AFutureGameExerciseCharacter* TargetCharacter)
{
	//---- Start of the Super section ----

	Character = TargetCharacter;

	if (Character == nullptr || Character->GetHasRifle())
	{
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	Character->SetHasRifle(true);

	//---- End of the Super section ----

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UHeatWeaponComponent::StartFire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UHeatWeaponComponent::StopFire);
		}
	}
}

void UHeatWeaponComponent::StartFire()
{
	UTP_WeaponComponent::Fire();

	if (IsValid(Character))
	{
		Character->GetWorldTimerManager().SetTimer(TimerHandle_HandleRefire, this, &UTP_WeaponComponent::Fire, FireInterval, true);
	}
}

void UHeatWeaponComponent::StopFire()
{
	if (IsValid(Character))
	{
		Character->GetWorldTimerManager().ClearTimer(TimerHandle_HandleRefire);
	}
}
