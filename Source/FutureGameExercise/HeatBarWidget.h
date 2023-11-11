#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeatBarWidget.generated.h"

class UTextBlock;
class UHeatWeaponComponent;

UCLASS()
class FUTUREGAMEEXERCISE_API UHeatBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* OverheatedText;

	UFUNCTION(BlueprintCallable)
	void SetWeapon(UHeatWeaponComponent* a_Weapon);

	UFUNCTION(BlueprintCallable)
	UHeatWeaponComponent* GetWeapon() const;

private:
	void NativeConstruct() override;

	UFUNCTION()
	void SetOverheatedTextVisibility();

	UHeatWeaponComponent* Weapon;
};
