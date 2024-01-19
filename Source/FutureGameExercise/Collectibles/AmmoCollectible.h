#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoCollectible.generated.h"

class UPickUpComponent;
class URotatingMovementComponent;
class UTextRenderComponent;
class AFutureGameExcerciseCharacter;

//Type of the pickup object
UENUM(BlueprintType)
enum class EAmmoType : uint8 //the type shouldn't be more than a byte
{
	None UMETA(Hidden),
	Ammo,
	Grenade
};

UCLASS()
class FUTUREGAMEEXERCISE_API AAmmoCollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmoCollectible();

	UFUNCTION()
	int TryTakeAmmo(int RequestedAmount);

	EAmmoType GetType() { return AmmoType; }

protected:
	void BeginPlay() override;

	void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "100"))
	int ContainingAmmo{ 5 };

	UPROPERTY(EditAnywhere, Category = Gameplay)
	EAmmoType AmmoType{ EAmmoType::Ammo };

	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
	TObjectPtr<UStaticMesh> GrenadeCollectibleMesh;

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
	TObjectPtr<UStaticMesh> AmmoCollectibleMesh;

	UPickUpComponent* PickUpComponent;

	URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
	float RotationSpeed{ 100 };

	UTextRenderComponent* ContainingAmmoText;

	UFUNCTION()
	void NotifyThePlayer(AMainCharacter* aCharacter);
};
