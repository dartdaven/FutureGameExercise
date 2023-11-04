// Fill out your copyright notice in the Description page of Project Settings.

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
	UAmmoCollectibleComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int mAmountOfAmmo;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FillAmmo(AFutureGameExerciseCharacter* TargetCharacter);
};
