#pragma once

#include "CoreMinimal.h"
#include "WeaponComponent.h"
#include "HeatWeaponComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FUTUREGAMEEXERCISE_API UHeatWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
	UHeatWeaponComponent();

	void SetupWeapon(AMainCharacter* a_Character) override;

	const float& GetTemperature() const;

	const float& GetMaxTemperature() const;

	bool IsOverheated() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverheatEvent);
	FOnOverheatEvent OverheatStateChanged;

protected:
	void Fire() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//User Adjustible Variables
	
	//Seconds to wait between shots
	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (ClampMin = "0", ClampMax = "2"))
	float FireInterval { 0.3f };

	//Temperature points per second
	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (ClampMin = "0"))
	float CooldownRate { 10.f };

	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (ClampMin = "0"))
	float TemperaturePerShot { 10.f };

	//Point of temperature when the weapon becomes not overheated anymore
	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (ClampMin = "0"))
	float CooldownPercentPoint { 0.2f };

	//Variables
	float Temperature { 0 };
	
	float MaxTemperature { 100.f };
	
	bool bIsOverheated { false };

	FTimerHandle TimerHandle_Cooldown;

	FCriticalSection TemperatureMutex;

	//Functions
	void StartFire();

	UFUNCTION() //Delegate callable
	void StopFire();

	void ClearOverheat();

	void SetupActionBindings() override;
	
	//Widget
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<class UUserWidget> HeatWidget;

	UPROPERTY(EditAnywhere, Category = Widget)
	FTransform WidgetTransform;

	class UWidgetComponent* HeatWidgetComponent;

	void SetupWidget();
};