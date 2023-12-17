#include "GrenadeCollectibleComponent.h"

int UGrenadeCollectibleComponent::TryTakeGrenades(int RequestedAmount)
{
	if (RequestedAmount < ContainingGrenades)
	{
		ContainingGrenades -= RequestedAmount;
		return RequestedAmount;
	}
	else
	{
		// Schedule destruction at the end of the frame
		GetWorld()->DestroyActor(GetOwner());

		return ContainingGrenades;
	}
}
