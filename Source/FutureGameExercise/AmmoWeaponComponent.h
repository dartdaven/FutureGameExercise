#pragma once

#include "CoreMinimal.h"
#include "TP_WeaponComponent.h"
#include "AmmoWeaponComponent.generated.h"

class UWidgetComponent;


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class FUTUREGAMEEXERCISE_API UAmmoWeaponComponent : public UTP_WeaponComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = "0", ClampMax = "20"))
	int MaxAmmo;

	void Reload();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	const int& GetCurrentAmmo() const;

	UAmmoWeaponComponent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoChange);
	FOnAmmoChange OnAmmoChange;

	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<class UUserWidget> AmmoWidget;

	//overriden
	void Fire() override;
	void AttachWeapon(AFutureGameExerciseCharacter* TargetCharacter) override;

private:

	UWidgetComponent* AmmoWidgetComponent;

	void SetupWidget();
	
	int CurrentAmmo;
};
