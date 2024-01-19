#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ThrowStrengthRadialWidget.generated.h"

UCLASS()
class FUTUREGAMEEXERCISE_API UThrowStrengthRadialWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class URadialSlider* RadialSlider;

	class AMainCharacter* Character;

public:
	void SetCharacter(AMainCharacter* aCharacter);

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
