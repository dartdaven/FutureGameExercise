#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

class AMainCharacter;
class UTextBlock;
class UAmmoWeaponComponent;

UCLASS()
class FUTUREGAMEEXERCISE_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RiffleAmmo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InventoryAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Customise, meta = (ClampMin = 0, ClampMax = 1))
	float RedTextPointPercent{ 0.2f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Customise, meta = (ClampMin = 0, ClampMax = 1))
	float YellowTextPointPercent{ 0.5f };
	
	UFUNCTION(BlueprintCallable)
	void SetWeapon(UAmmoWeaponComponent* a_Weapon);

	UFUNCTION(BlueprintCallable)
	UAmmoWeaponComponent* GetWeapon() const;

private:
	UFUNCTION()
	void Refresh();

	UAmmoWeaponComponent* Weapon;
};
