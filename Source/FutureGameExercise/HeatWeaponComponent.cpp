#include "HeatWeaponComponent.h"

#include "FutureGameExerciseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "HelpingTools.h"

void UHeatWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Temperature = FMath::Clamp(Temperature - (DeltaTime * CooldownRate), 0, MaxTemperature);
}

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
	//Check to not to overcome FireInterval
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Cooldown))
	{	
		float CooldownTime = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Cooldown);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefire, this, &UHeatWeaponComponent::StartFire, CooldownTime, false);
		
		return;
	}

	Fire();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefire, this, &UHeatWeaponComponent::Fire, FireInterval, true);
}

void UHeatWeaponComponent::StopFire()
{
	float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_HandleRefire);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HandleRefire);
	
	if (!IsOverheated())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, TimeRemaining, false);
	}
}

void UHeatWeaponComponent::Fire()
{
	if (IsOverheated())
	{
		return;
	}

	UTP_WeaponComponent::Fire();

	FScopeLock Lock(&TemperatureMutex);

	Temperature += TemperaturePerShot;

	if (Temperature >= MaxTemperature)
	{
		Temperature = MaxTemperature;

		bIsOverheated = true;

		OnOverheatEvent.Broadcast();

		float TimeToCooldown = MaxTemperature * (1 - CooldownPercentPoint) / CooldownRate;

		//It may be a potential problem to use the same Timer as between shots, but I don't face it
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, this, &UHeatWeaponComponent::ClearOverheat, TimeToCooldown, false);
	}
}

const float& UHeatWeaponComponent::GetTemperature() const
{
	return Temperature;
}

const float& UHeatWeaponComponent::GetMaxTemperature() const
{
	return MaxTemperature;
}

bool UHeatWeaponComponent::IsOverheated() const
{
	return bIsOverheated;
}

void UHeatWeaponComponent::ClearOverheat()
{
	bIsOverheated = false;

	OnOverheatEvent.Broadcast();
}