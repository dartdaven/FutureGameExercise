#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

class UProjectileMovementComponent;
class URadialForceComponent;

UCLASS()
class FUTUREGAMEEXERCISE_API AGrenade : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
	USoundBase* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
	UParticleSystem* ExplosionVFX;

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
	UParticleSystem* TrailVFX;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly)
	URadialForceComponent* RadialForce;

	FTimerHandle TimerHandle_Explode;
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

	//Getters
	UStaticMeshComponent* GetMesh() const { return Mesh; }

	/** 
	* Percent from MaxSpeed
	* Pass value from 0 to 1
	*/
	void SetInitialSpeedPercent(float aInitialSpeed);

protected:

	void BeginPlay() override;

	void SetupGrenade();
	
	void Explode();

	UPROPERTY(EditDefaultsOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true", ClampMin = "0.1", ClampMax = "20"))
	float TimeToExplode{ 2.5f };

	UPROPERTY(EditDefaultsOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "100"))
	int OnHitImpulseMultiplier{ 40 };

};
