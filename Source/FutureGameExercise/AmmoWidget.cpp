#include "AmmoWidget.h"

#include "Components/TextBlock.h" 

#include "AmmoWeaponComponent.h"
#include "FutureGameExerciseCharacter.h"
#include "HelpingTools.h"

void UAmmoWidget::SetWeapon(UAmmoWeaponComponent* a_Weapon)
{
	Weapon = a_Weapon;

	if (IsValid(Weapon))
	{
		Weapon->OnAmmoChange.AddDynamic(this, &UAmmoWidget::Refresh);

		if (IsValid(Weapon->GetCharacter()))
		{
			Weapon->GetCharacter()->OnAmmoChange.AddDynamic(this, &UAmmoWidget::Refresh);

			Refresh();

			Help::DisplayDebugMessage(TEXT("Ammo Widget is successfully set"));

			return;
		}
	}

	Help::DisplayErrorMessage(TEXT("Ammo Widget is not set correctly"));
}

UAmmoWeaponComponent* UAmmoWidget::GetWeapon() const
{
	return Weapon;
}

void UAmmoWidget::Refresh()
{
	RiffleAmmo->SetText(FText::AsNumber(Weapon->GetCurrentAmmo()));
	InventoryAmmo->SetText(FText::AsNumber(Weapon->GetCharacter()->GetAmmoAmount()));

	float AmmoPercent = static_cast<float>(Weapon->GetCurrentAmmo()) / Weapon->GetMaxAmmo();

	if (AmmoPercent < YellowTextPointPercent)
	{
		if (AmmoPercent <= RedTextPointPercent)
		{
			RiffleAmmo->SetColorAndOpacity(FLinearColor::Red);
			return;
		}

		RiffleAmmo->SetColorAndOpacity(FLinearColor::Yellow);
	}
	else
	{
		RiffleAmmo->SetColorAndOpacity(FLinearColor::White);
	}
}