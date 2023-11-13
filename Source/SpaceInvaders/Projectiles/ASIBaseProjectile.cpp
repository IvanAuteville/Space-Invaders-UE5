// Copyright Ivan Ariel Garcia Auteville.


#include "ASIBaseProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvaders/GameModes/ASIGameModeBase.h"

AASIBaseProjectile::AASIBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetMobility(EComponentMobility::Movable);

	MeshComp->SetGenerateOverlapEvents(false);
	MeshComp->SetNotifyRigidBodyCollision(false);
	MeshComp->CanCharacterStepUpOn = ECB_No;

	// Save the relative offset location from the Root Scene Component
	// since the ProjectileMovementComp will be updating the MeshComp position, but not the Root
	MeshOffset = MeshComp->GetRelativeLocation();

	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// Object Type: WorldDynamic
	// Ignore everything, Block World Static and Dynamic

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->MaxSpeed = Speed;
	ProjectileMovementComp->InitialSpeed = Speed;
		
	// TODO: VFX
	//TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	//TrailParticles->SetupAttachment(RootComponent);
}

void AASIBaseProjectile::Enable()
{
	// Reset Mesh Component offset location from the Root Scene Component
	MeshComp->SetRelativeLocation(MeshOffset);

	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	ProjectileMovementComp->Activate();
	ProjectileMovementComp->SetUpdatedComponent(MeshComp);
}

void AASIBaseProjectile::Disable()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	ProjectileMovementComp->Deactivate();
	ProjectileMovementComp->StopMovementImmediately();
}

void AASIBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComp->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);

	AASIGameModeBase* MyGameMode = Cast<AASIGameModeBase>(UGameplayStatics::GetGameMode(this));
	MyGameMode->OnGamePaused.AddDynamic(this, &ThisClass::OnGamePaused);

	Disable();

	// TODO: Sound
	// UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

void AASIBaseProjectile::OnGamePaused(const bool bPaused)
{
	ProjectileMovementComp->StopMovementImmediately();
	SetActorTickEnabled(!bPaused);
	bEnabled = !bPaused;
}

void AASIBaseProjectile::Fire()
{
	if(!bEnabled)
		return;
	
	Enable();
	ProjectileMovementComp->SetVelocityInLocalSpace(FVector(0.0, -Speed, 0.0));
}

void AASIBaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (MyOwner == nullptr)
		return;

	if (OtherActor == nullptr || OtherActor == this || OtherActor == MyOwner)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Projectile Collision with %s"), *OtherActor->GetName());

	IDestructible* Destructible = Cast<IDestructible>(OtherActor);
	if (Destructible)
	{
		Destructible->HandleDestruction(this);
	}

	// "Self Destruct" (Pooled: Hide, Disable, etc)
	HandleDestruction(OtherActor);
	
	if (!OnProjectileHit.IsBound())
		return;

	OnProjectileHit.Broadcast();
}

void AASIBaseProjectile::HandleDestruction(AActor* DestroyerActor)
{
	// TODO: VFX
	//UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());

	// TODO: Sound
	//UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());

	Disable();
}