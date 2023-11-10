#include "HeatWeaponComponent.h"

#include "FutureGameExerciseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "HelpingTools.h"

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
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Cooldown))
	{
		//Help::DisplayDebugMessage(TEXT("Cooldown is pending"));
		float CooldownTime = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Cooldown);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefire, this, &UHeatWeaponComponent::StartFire, CooldownTime, false);
		
		return;
	}

	UTP_WeaponComponent::Fire();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefire, this, &UTP_WeaponComponent::Fire, FireInterval, true);
}

void UHeatWeaponComponent::StopFire()
{
	float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_HandleRefire);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HandleRefire);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, TimeRemaining, false);
	//Help::DisplayDebugMessage(TEXT("Timer is set to %f seconds"), TimeRemaining);
}
