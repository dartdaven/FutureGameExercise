#include "Grenade.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

#include "HelpingTools.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetNotifyRigidBodyCollision(true); // For generation of Hit events 
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); //Ignore player collision,  
	Mesh->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	//Default parameters. Can be adjusted in blueprint
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
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
	if (ExplosionVFX != nullptr)
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

void AGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * OnHitImpulseMultiplier, GetActorLocation());
	}
}
