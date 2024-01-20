#include "Grenade.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Particles/ParticleSystem.h"

#include "../Misc/HelpingTools.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); //Ignore player collision,  

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	//Default parameters. Can be adjusted in blueprint
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	
	//I don't know why, if it's not attached to mesh the explosion will be at the begining of the coordinates
	RadialForce->SetupAttachment(Mesh);
	
	//Defaults parameters. Can be adjusted in blueprint
	RadialForce->bImpulseVelChange = true;
	RadialForce->Radius = 800.0f;
	RadialForce->ImpulseStrength = 500.0f;
}

void AGrenade::SetInitialSpeedPercent(float aInitialSpeed)
{
	ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed * FMath::Clamp(aInitialSpeed, 0, 1);
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	SetupGrenade();
}

void AGrenade::SetupGrenade()
{
	ProjectileMovement->SetUpdatedComponent(Mesh);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Explode, this, &AGrenade::Explode, TimeToExplode, false);
}

void AGrenade::Explode()
{
	if (IsValid(ExplosionVFX))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFX, GetActorLocation());
	}

	if (IsValid(ExplosionSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}

	RadialForce->FireImpulse();

	Destroy();
}
