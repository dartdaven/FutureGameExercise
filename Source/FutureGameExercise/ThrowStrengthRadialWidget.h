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

	class AFutureGameExerciseCharacter* Character;

public:
	void SetCharacter(AFutureGameExerciseCharacter* aCharacter);

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
