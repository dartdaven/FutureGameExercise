// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrenadeCollectibleComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FUTUREGAMEEXERCISE_API UGrenadeCollectibleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	int ContainingGrenades{ 3 };

	UFUNCTION()
	int TryTakeGrenades(int RequestedAmount);
};
