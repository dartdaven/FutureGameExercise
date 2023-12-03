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
		// Schedule destruction at the end of the frame
		GetWorld()->DestroyActor(GetOwner());

		return ContainingAmmo;
	}
}

