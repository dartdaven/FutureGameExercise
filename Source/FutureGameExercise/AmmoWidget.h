#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

class AFutureGameExerciseCharacter;
class UTextBlock;
class UTP_WeaponComponent;


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
	void SetWeapon(UTP_WeaponComponent* a_Weapon);

	UFUNCTION(BlueprintCallable)
	UTP_WeaponComponent* GetWeapon() const;

//protected:
//	void NativeOnInitialized() override;

private:
	UTP_WeaponComponent* Weapon;
};
