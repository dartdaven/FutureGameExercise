#pragma once

#include "CoreMinimal.h"
#include "TP_WeaponComponent.h"
#include "HeatWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverheatEvent);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FUTUREGAMEEXERCISE_API UHeatWeaponComponent : public UTP_WeaponComponent
{
	GENERATED_BODY()

public:
	//Seconds to wait between shots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = "0", ClampMax = "2"))
	float FireInterval {0.3f};

	//Temperature points per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = "0"))
	float CooldownRate { 10.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = "0"))
	float TemperaturePerShot{ 10.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = "0"))
	float CooldownPercentPoint { 0.2f };

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool SetupActionBindings() override;
	
	void StartFire();
	void StopFire();
	void Fire() override;

	UFUNCTION(BlueprintCallable)
	const float& GetTemperature() const;

	UFUNCTION(BlueprintCallable)
	const float& GetMaxTemperature() const;

	UFUNCTION(BlueprintCallable)
	bool IsOverheated() const;

	FOnOverheatEvent OverheatStateChanged;

private:
	float Temperature {};
	float MaxTemperature { 100.f };
	bool bIsOverheated { false };

	void ClearOverheat();

	FTimerHandle TimerHandle_HandleRefire;
	FTimerHandle TimerHandle_Cooldown;

	FCriticalSection TemperatureMutex;
};