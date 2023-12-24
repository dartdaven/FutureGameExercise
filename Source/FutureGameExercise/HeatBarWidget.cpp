#include "HeatBarWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "HeatWeaponComponent.h"
#include "HelpingTools.h"


void UHeatBarWidget::SetWeapon(UHeatWeaponComponent* a_Weapon)
{
	Weapon = a_Weapon;

	if (Weapon != nullptr)
	{
		Weapon->OverheatStateChanged.AddDynamic(this, &UHeatBarWidget::SetOverheatedTextVisibility);
	}
	else
	{
		Help::DisplayErrorMessage(TEXT("Weapon is not set"));
	}
}

UHeatWeaponComponent* UHeatBarWidget::GetWeapon() const
{
	return Weapon;
}

void UHeatBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float Percent = Weapon->GetTemperature() / Weapon->GetMaxTemperature();
	TemperatureBar->SetPercent(Percent);
}

void UHeatBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Weapon != nullptr)
	{
		Weapon->OverheatStateChanged.AddDynamic(this, &UHeatBarWidget::SetOverheatedTextVisibility);
	}

	OverheatedText->SetVisibility(ESlateVisibility::Hidden);
}

void UHeatBarWidget::SetOverheatedTextVisibility()
{
	if (Weapon != nullptr)
	{
		if (Weapon->IsOverheated())
		{

			OverheatedText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			OverheatedText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
