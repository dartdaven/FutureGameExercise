#include "AmmoCollectibleComponent.h"

#include "FutureGameExerciseCharacter.h"

UAmmoCollectibleComponent::UAmmoCollectibleComponent()
{
	mAmountOfAmmo = 5;
}

void UAmmoCollectibleComponent::FillAmmo(AFutureGameExerciseCharacter* TargetCharacter)
{
	// Check that the character is valid
	if (TargetCharacter == nullptr)
	{
		return;
	}
	
	int ammoNeeded = TargetCharacter->mMaxAmmoAmount - TargetCharacter->GetAmmoAmount();
	
	if (ammoNeeded < mAmountOfAmmo)
	{
		TargetCharacter->FillAmmo(ammoNeeded);
		mAmountOfAmmo -= ammoNeeded;
	}
	else
	{
		TargetCharacter->FillAmmo(mAmountOfAmmo);

		if (GetOwner() != nullptr)
		{
			GetOwner()->Destroy();
		}
	}
}

