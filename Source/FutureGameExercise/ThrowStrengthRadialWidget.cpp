#include "ThrowStrengthRadialWidget.h"

#include "Components/RadialSlider.h"
#include "Kismet/GameplayStatics.h"

#include "FutureGameExerciseCharacter.h"
#include "HelpingTools.h"


void UThrowStrengthRadialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(RadialSlider))
	{
		//Defaults, can be adjusted in Blueprint
		RadialSlider->SetSliderHandleStartAngle(0.0f);
		RadialSlider->SetSliderHandleEndAngle(361.0f);
		RadialSlider->SetAngularOffset(180.f);
	
		FColor Transparent = FColor(1, 1, 1, 0);
		RadialSlider->SetSliderBarColor(Transparent);
	
		RadialSlider->SetShowSliderHandle(false);
		RadialSlider->WidgetStyle.BarThickness = 25;
	}

}

void UThrowStrengthRadialWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(Character))
	{
		RadialSlider->SetValue(Character->GetThrowStrength());
	}
}

void UThrowStrengthRadialWidget::SetCharacter(AFutureGameExerciseCharacter* aCharacter)
{
	Character = aCharacter;
}