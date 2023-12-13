// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FutureGameExerciseCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class UAmmoCollectibleComponent;
class UTP_WeaponComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFutureGameExerciseCharacter : public ACharacter
{
	GENERATED_BODY()

	/* First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/* Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/* Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowGrenadeAction;

	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	TSubclassOf<class AGrenade> GrenadeClass;
	
public:
	AFutureGameExerciseCharacter();
		
	/* Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/* Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/* Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/* Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	int TakeOutAmmo(const int& amountRequested);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	const int& GetAmmoAmount() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoChange);
	FOnAmmoChange OnAmmoChange;

	UFUNCTION(BlueprintCallable, Category = Interaction)
	void OnAmmoPickUp(UAmmoCollectibleComponent* AmmoComponent);

	UFUNCTION(BlueprintCallable, Category = Interaction)
	void OnWeaponPickUp(UTP_WeaponComponent* WeaponComponent);

protected:
	/* Called for movement input */
	void Move(const FInputActionValue& Value);

	/* Called for looking input */
	void Look(const FInputActionValue& Value);

	/* APawn interface */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void SwitchWeapon();

	void ActivateWeapon(UTP_WeaponComponent* WeaponToActivate);

	void DeactivateWeapon(UTP_WeaponComponent* WeaponToDeactivate);

	void ThrowGrenade();


private:
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchWeaponAction;

	UTP_WeaponComponent* ActiveWeapon = nullptr;

	TArray<UTP_WeaponComponent*> Weapons;

	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "100"))
	int MaxAmmoAmount;
	
	int AmmoAmount;

	UPROPERTY(EditDefaultsOnly, Category = Grenade, meta = (AllowPrivateAccess = "true"))
	USceneComponent* GrenadeSpawnPoint;
};