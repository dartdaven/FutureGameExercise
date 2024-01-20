#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponComponent.generated.h"

class AMainCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FUTUREGAMEEXERCISE_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AAmmoProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* WeaponMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Sets default values for this component's properties */
	UWeaponComponent();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual void Fire();

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** The Character holding this weapon*/
	AMainCharacter* Character;


	/* --- Custom code section --- */

public:
	//Is needed for the character to bind and unbind the weapon mappings
	const UInputMappingContext* GetMappingContext() const;
	
	AMainCharacter* GetCharacter() const;

	FString GetWeaponName() const { return WeaponName; }

	virtual void SetupWeapon(AMainCharacter* a_Character);

	virtual void SetupActionBindings();

	void ClearActionBindings();

protected:
	void SetCharacter(AMainCharacter* a_Character);

	bool FireImpl();

	UPROPERTY(EditAnywhere /*For instances to have their own name*/, Category = Gameplay)
	FString WeaponName{ TEXT("Default Weapon") };
};
