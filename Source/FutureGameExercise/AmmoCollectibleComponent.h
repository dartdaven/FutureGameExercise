#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "AmmoCollectibleComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FUTUREGAMEEXERCISE_API UAmmoCollectibleComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	int ContainingAmmo { 5 };

	UFUNCTION()
	int TryTakeAmmo(int RequestedAmount);
};
