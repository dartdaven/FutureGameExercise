// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "TP_AmmoComponent.generated.h"

class AFutureGameExerciseCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FUTUREGAMEEXERCISE_API UTP_AmmoComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UTP_AmmoComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int mAmountOfAmmo;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FillAmmo(AFutureGameExerciseCharacter* TargetCharacter);
};
