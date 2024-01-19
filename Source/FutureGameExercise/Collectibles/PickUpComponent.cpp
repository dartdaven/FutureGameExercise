// Copyright Epic Games, Inc. All Rights Reserved.

#include "PickUpComponent.h"

#include "../Misc/HelpingTools.h"

UPickUpComponent::UPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UPickUpComponent::OnSphereBeginOverlap);
}

void UPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Help::DisplayDebugMessage(TEXT("Overlap happened"));

	// Checking if it is a First Person Character overlapping
	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if(IsValid(Character))
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		//OnComponentBeginOverlap.RemoveAll(this);
	}
}
