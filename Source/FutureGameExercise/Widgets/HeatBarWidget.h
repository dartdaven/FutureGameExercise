#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeatBarWidget.generated.h"

class UTextBlock;
class UHeatWeaponComponent;
class UProgressBar;

UCLASS()
class FUTUREGAMEEXERCISE_API UHeatBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* OverheatedText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* TemperatureBar;

	UFUNCTION(BlueprintCallable)
	void SetWeapon(UHeatWeaponComponent* a_Weapon);

	UFUNCTION(BlueprintCallable)
	UHeatWeaponComponent* GetWeapon() const;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void NativeConstruct() override;

	UFUNCTION()
	void SetOverheatedTextVisibility();

	UHeatWeaponComponent* Weapon;
};
