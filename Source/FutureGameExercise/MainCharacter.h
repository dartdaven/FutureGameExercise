// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MainCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

class AAmmoCollectible;
class UWeaponComponent;
class UThrowStrengthRadialWidget;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMainCharacter : public ACharacter
{
	/* --- Default code section --- */

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

	
public:
	AMainCharacter();
		
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


protected:
	/* Called for movement input */
	void Move(const FInputActionValue& Value);

	/* Called for looking input */
	void Look(const FInputActionValue& Value);

	/* APawn interface */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	
	/* --- Custom code section --- */

public:

	UFUNCTION(BlueprintCallable, Category = Inventory)
	int TakeOutAmmo(const int& amountRequested);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	const int& GetAmmoAmount() const;

	const float& GetThrowStrength() const { return ThrowStrength; };

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoChange);
	FOnAmmoChange OnAmmoChange;

	UFUNCTION(BlueprintCallable, Category = Interaction)
	void OnAmmoPickUp(AAmmoCollectible* AmmoComponent);

	UFUNCTION(BlueprintCallable, Category = Interaction)
	void OnWeaponPickUp(UWeaponComponent* WeaponComponent);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponSwitch);
	FOnWeaponSwitch OnWeaponSwitch;

private:
	//Weapon switching
	TArray<UWeaponComponent*> Weapons;

	UWeaponComponent* ActiveWeapon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchWeaponAction;
	
	void SwitchWeapon();

	void ActivateWeapon(UWeaponComponent* WeaponToActivate);

	void DeactivateWeapon(UWeaponComponent* WeaponToDeactivate);

	//Inventory
	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "100"))
	int MaxAmmoAmount { 30 };
	
	int AmmoAmount { 0 };

	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "20"))
	int MaxGrenadeAmount { 5 };

	int GrenadeAmount { 0 };

	//Grenade
	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	USceneComponent* GrenadeSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ThrowGrenadeAction;

	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	TSubclassOf<class AGrenade> GrenadeClass;

	void StartThrowingGrenade();

	void StopThrowingGrenade();

	void ThrowGrenade();	

	float ThrowStrength{ 0 };

	void IncrementThrowStrength();

	UPROPERTY(EditDefaultsOnly, Category = Grenade, meta = (AllowPrivateAccess = "true", ClampMin = "0.1", ClampMax = "3"))
	float SecondsToMaxThrowStrength{ 0.8f };

	float SecondsBetweenIncrementingThrowStrength{ 0.03f };

	bool bHasGrenade{ false };

	FTimerHandle TimerHandle_ThrowStrengthIncreaser;

	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	TSubclassOf<UThrowStrengthRadialWidget> ThrowWidgetClass;

	UThrowStrengthRadialWidget* ThrowWidget;
};