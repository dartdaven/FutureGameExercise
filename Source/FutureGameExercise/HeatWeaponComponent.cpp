#include "HeatWeaponComponent.h"

#include "FutureGameExerciseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"

#include "HeatBarWidget.h"
#include "HelpingTools.h"

void UHeatWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FScopeLock Lock(&TemperatureMutex);

	Temperature = FMath::Clamp(Temperature - (DeltaTime * CooldownRate), 0, MaxTemperature);
}

bool UHeatWeaponComponent::SetupActionBindings()
{
	if (!IsValid(Character))
	{
		Help::DisplayErrorMessage(TEXT("Unable to setup bindings due to the character is absent"));
		return false;
	}

	SetupWidget(); //TODO Get it out of here

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(WeaponMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UHeatWeaponComponent::StartFire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UHeatWeaponComponent::StopFire);

			return true;
		}
	}

	return false;
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

	if (UTP_WeaponComponent::FireImpl())
	{
		FScopeLock Lock(&TemperatureMutex);

		Temperature += TemperaturePerShot;

		if (Temperature >= MaxTemperature)
		{
			Temperature = MaxTemperature;

			bIsOverheated = true;

			OverheatStateChanged.Broadcast();

			float TimeToCooldown = MaxTemperature * (1 - CooldownPercentPoint) / CooldownRate;

			//It may be a potential problem to use the same Timer as between shots, but I don't face it
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, this, &UHeatWeaponComponent::ClearOverheat, TimeToCooldown, false);
		}
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

UHeatWeaponComponent::UHeatWeaponComponent()
{
	HeatWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeatWidgetComponent"));
}

void UHeatWeaponComponent::ClearOverheat()
{
	bIsOverheated = false;

	OverheatStateChanged.Broadcast();
}

void UHeatWeaponComponent::SetupWidget()
{
	//MagicNumbers Alert
	HeatWidgetComponent->SetRelativeLocationAndRotation(FVector(-15.f, 45.f, 20.f), FQuat(FRotator(0.f, 270.f, 0.f)));
	HeatWidgetComponent->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));

	HeatWidgetComponent->SetCastShadow(false);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	HeatWidgetComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	HeatWidgetComponent->SetWidgetClass(HeatWidget);

	UHeatBarWidget* HeatBarWidgetPtr = Cast<UHeatBarWidget>(HeatWidgetComponent->GetUserWidgetObject());

	if (IsValid(HeatBarWidgetPtr))
	{
		HeatBarWidgetPtr->SetWeapon(this);
	}
	else
	{
		Help::DisplayErrorMessage(TEXT("Widget class is not set correctly in the Weapon Blueprint"));
	}
}
