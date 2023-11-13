#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "AmmoCollectibleComponent.generated.h"

class AFutureGameExerciseCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FUTUREGAMEEXERCISE_API UAmmoCollectibleComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int ContainingAmmo { 5 };

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int TryTakeAmmo(int RequestedAmount);
};
