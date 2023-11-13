#include "AmmoCollectibleComponent.h"

#include "FutureGameExerciseCharacter.h"

int UAmmoCollectibleComponent::TryTakeAmmo(int RequestedAmount)
{
	if (RequestedAmount < ContainingAmmo)
	{
		ContainingAmmo -= RequestedAmount;
		return RequestedAmount;
	}
	else
	{
		//It may be a huge problem
		if (GetOwner() != nullptr)
		{
			GetOwner()->Destroy();
		}

		return ContainingAmmo;
	}
}

