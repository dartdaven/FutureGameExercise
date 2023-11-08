#include "AmmoWidget.h"

//#include "FutureGameExerciseCharacter.h"
#include "Components/TextBlock.h" 

#include "HelpingTools.h"

void UAmmoWidget::SetWeapon(UAmmoWeaponComponent* a_Weapon)
{
	Weapon = a_Weapon;
}

UAmmoWeaponComponent* UAmmoWidget::GetWeapon() const
{
	return Weapon;
}

//void UAmmoWidget::NativeOnInitialized()
//{
//	Character = Cast<AFutureGameExerciseCharacter>(GetOwningPlayerPawn());
//	if (Character)
//	{
//		RefreshInfo();
//		Character->OnFillAmmo.AddDynamic(this, &UAmmoWidget::RefreshInfo);
//	}
//}
//
//void UAmmoWidget::RefreshInfo()
//{
//	Help::DisplayDebugMessage(TEXT("Delegate works"));
//
//	if (Character)
//	{
//		InventoryAmmo->SetText(FText::AsNumber(Character->GetAmmoAmount()));
//	}
//}