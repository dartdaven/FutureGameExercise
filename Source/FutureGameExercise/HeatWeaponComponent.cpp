#include "HeatWeaponComponent.h"

#include "FutureGameExerciseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"

#include "HeatBarWidget.h"
#include "HelpingTools.h"

UHeatWeaponComponent::UHeatWeaponComponent()
{
	HeatWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeatWidgetComponent"));

	//Default parameters can be adjusted in blueprint
	WidgetTransform = FTransform(FRotator(0.f, 270.f, 0.f), FVector(-15.f, 45.f, 20.f), FVector(0.15f));

	WeaponName = TEXT("Default Heat Weapon");
}

void UHeatWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FScopeLock Lock(&TemperatureMutex);

	Temperature = FMath::Clamp(Temperature - (DeltaTime * CooldownRate), 0, MaxTemperature);
}

void UHeatWeaponComponent::SetupWeapon()
{
	SetupWidget();

	Character->OnWeaponSwitch.AddDynamic(this, &UHeatWeaponComponent::StopFire);

	SetupActionBindings();
}

void UHeatWeaponComponent::SetupActionBindings()
{
	if (!IsValid(Character))
	{
		Help::DisplayErrorMessage(TEXT("%s: Unable to setup bindings due to the character is absent"), *GetNameSafe(this));
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
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
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();	
	float CooldownTime = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Cooldown);
	
	if (bIsOverheated)
	{
		//it's interesting what I can write instead of auto and not an std::function
		auto OverheatCallback = [this]()
		{
			ClearOverheat();
			StartFire();
		};

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, OverheatCallback, CooldownTime, false);

		return;
	}
	else if	(TimerManager.GetTimerRemaining(TimerHandle_Cooldown) >= SMALL_NUMBER) //float problem
	{	
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, this, &UHeatWeaponComponent::StartFire, CooldownTime, false);
		
		return;
	}

	Fire();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, this, &UHeatWeaponComponent::Fire, FireInterval, true);
}

void UHeatWeaponComponent::StopFire()
{
	float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Cooldown);
	
	if (!bIsOverheated)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Cooldown);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, TimeRemaining, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, this, &UHeatWeaponComponent::ClearOverheat, TimeRemaining, false);
	}
}

void UHeatWeaponComponent::Fire()
{
	if (IsOverheated())
	{
		//TODO sound unable to fire

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

void UHeatWeaponComponent::ClearOverheat()
{
	bIsOverheated = false;

	OverheatStateChanged.Broadcast();
}

void UHeatWeaponComponent::SetupWidget()
{
	HeatWidgetComponent->SetRelativeTransform(WidgetTransform);

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
