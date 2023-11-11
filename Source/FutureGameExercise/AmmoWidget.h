#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

class AFutureGameExerciseCharacter;
class UTextBlock;
class UAmmoWeaponComponent;

UCLASS()
class FUTUREGAMEEXERCISE_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
	
	//UFUNCTION()
	//void RefreshInfo();

public:
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* RiffleAmmo;

	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* InventoryAmmo;

	//AFutureGameExerciseCharacter* Character;

	UFUNCTION(BlueprintCallable)
	void SetWeapon(UAmmoWeaponComponent* a_Weapon);

	UFUNCTION(BlueprintCallable)
	UAmmoWeaponComponent* GetWeapon() const;

//protected:
//	void NativeOnInitialized() override;

private:
	UAmmoWeaponComponent* Weapon;
};
