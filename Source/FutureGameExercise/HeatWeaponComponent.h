#pragma once

#include "CoreMinimal.h"
#include "TP_WeaponComponent.h"
#include "HeatWeaponComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FUTUREGAMEEXERCISE_API UHeatWeaponComponent : public UTP_WeaponComponent
{
	GENERATED_BODY()

public:
	//seconds to wait between shots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = "0", ClampMax = "2"))
	float FireInterval {0.5f};
	
	void AttachWeapon(AFutureGameExerciseCharacter* TargetCharacter) override;
	
	void StartFire();
	void StopFire();

	void GetTemperature() const;

private:
	int Temperature;
	int MaxTemperature;
	bool bIsOverheated;

	FTimerHandle TimerHandle_HandleRefire;
	FTimerHandle TimerHandle_Cooldown;
};