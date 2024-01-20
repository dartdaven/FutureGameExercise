#include "AmmoCollectible.h"

#include "GameFramework/RotatingMovementComponent.h"
#include "Components/TextRenderComponent.h"

#include "PickUpComponent.h"
#include "../Misc/HelpingTools.h"
#include "../MainCharacter.h"

// Sets default values
AAmmoCollectible::AAmmoCollectible()
{
	PrimaryActorTick.bCanEverTick = true;

	PickUpComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("PickUpComponent"));
	RootComponent = PickUpComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0, RotationSpeed, 0);
	RotatingMovement->UpdatedComponent = Mesh;

	ContainingAmmoText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ContainingAmmoText"));
	ContainingAmmoText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	ContainingAmmoText->SetupAttachment(RootComponent);
}

void AAmmoCollectible::BeginPlay()
{
	Super::BeginPlay();

	bool bSuccesfullyCreated = false;

	if (AmmoType == EAmmoType::Ammo)
	{
		if (IsValid(AmmoCollectibleMesh))
		{
			Mesh->SetStaticMesh(AmmoCollectibleMesh);
			
			bSuccesfullyCreated = true;
		}
		else
		{
			Help::DisplayErrorMessage(TEXT("%s: There is no Ammo mesh for Ammo type"), *GetNameSafe(this));
		}
	}
	else if (AmmoType == EAmmoType::Grenade)
	{
		if (IsValid(GrenadeCollectibleMesh))
		{
			Mesh->SetStaticMesh(GrenadeCollectibleMesh);
			ContainingAmmoText->SetVisibility(false);

			bSuccesfullyCreated = true;
		}
		else
		{
			Help::DisplayErrorMessage(TEXT("%s: There is no Grenade mesh for Grenade type"), *GetNameSafe(this));
		}
	}
	else
	{
		Help::DisplayErrorMessage(TEXT("%s: Not known ammo type is selected"), *GetNameSafe(this));
	}

	if (bSuccesfullyCreated)
	{
		PickUpComponent->OnPickUp.AddDynamic(this, &AAmmoCollectible::NotifyThePlayer);
	}
}

// Called every frame
void AAmmoCollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator Yaw = FRotator(0, GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorRotation().Yaw + 180, 0);
	ContainingAmmoText->SetRelativeRotation(Yaw);
	ContainingAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ContainingAmmo))); //TODO find a better way to convert
}

void AAmmoCollectible::NotifyThePlayer(AMainCharacter* aCharacter)
{
	if (IsValid(aCharacter))
	{
		aCharacter->OnAmmoPickUp(this);
	}
}

int AAmmoCollectible::TryTakeAmmo(int RequestedAmount)
{
	if (RequestedAmount < ContainingAmmo)
	{
		Help::DisplayDebugMessage(TEXT("%s: Your ammo is full"), *GetNameSafe(this));

		ContainingAmmo -= RequestedAmount;
		return RequestedAmount;
	}
	else
	{
		// Schedule destruction at the end of the frame
		GetWorld()->DestroyActor(this);

		return ContainingAmmo;
	}
}