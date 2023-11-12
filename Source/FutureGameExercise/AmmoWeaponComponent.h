#pragma once

#include "CoreMinimal.h"
#include "TP_WeaponComponent.h"
#include "AmmoWeaponComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class FUTUREGAMEEXERCISE_API UAmmoWeaponComponent : public UTP_WeaponComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = "0", ClampMax = "20"))
	int MaxAmmo;

	void Reload();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	const int& GetCurrentAmmo() const;

	UAmmoWeaponComponent();

	//overriden
	void Fire() override;
	void AttachWeapon(AFutureGameExerciseCharacter* TargetCharacter) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoChange);
	FOnAmmoChange OnAmmoChange;

private:

	int CurrentAmmo;
};
