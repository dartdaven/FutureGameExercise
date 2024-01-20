#pragma once

#include "CoreMinimal.h"
#include "WeaponComponent.h"
#include "AmmoWeaponComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class FUTUREGAMEEXERCISE_API UAmmoWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
	UAmmoWeaponComponent();

	const int& GetCurrentAmmo() const;

	const int& GetMaxAmmo() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoChange);
	FOnAmmoChange OnAmmoChange;

protected:
	void InitializeComponent() override;

	void Fire() override;

	void SetupWeapon(AMainCharacter* a_Character) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (ClampMin = "0", ClampMax = "20"))
	int MaxAmmo{ 10 };

	UPROPERTY()
	int CurrentAmmo;
	
	//Functions
	void Reload();
	
	void SetupActionBindings();

	//Widget
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<class UUserWidget> AmmoWidget;
	
	UPROPERTY(EditAnywhere, Category = Widget)
	FTransform WidgetTransform;

	class UWidgetComponent* AmmoWidgetComponent;

	void SetupWidget();
};
