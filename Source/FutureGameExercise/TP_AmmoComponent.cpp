#include "TP_AmmoComponent.h"

#include "FutureGameExerciseCharacter.h"

UTP_AmmoComponent::UTP_AmmoComponent()
{
	mAmountOfAmmo = 5;
}

void UTP_AmmoComponent::FillAmmo(AFutureGameExerciseCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no rifle yet
	if (Character == nullptr)
	{
		return;
	}

	Character->FillAmmo(mAmountOfAmmo);

}
