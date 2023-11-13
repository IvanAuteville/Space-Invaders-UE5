// Copyright Ivan Ariel Garcia Auteville.


#include "ASIInvaderActor.h"
#include "SpaceInvaders/Enemies/ASIInvadersFormation.h"
#include "SpaceInvaders/Player/ASIPlayerPawn.h"

AASIInvaderActor::AASIInvaderActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Init Components
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComp);
	StaticMeshComp->SetGenerateOverlapEvents(true);
	StaticMeshComp->SetNotifyRigidBodyCollision(false);
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// Object Type: WorldDynamic
	// Ignore everything, Block WorldStatic (barriers, bunkers) and WorldDynamic (projectiles), Pawn

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(StaticMeshComp);
}

void AASIInvaderActor::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

// TODO: check if neccesary
void AASIInvaderActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr || OtherActor == this)
		return;
}

void AASIInvaderActor::SetFormation(AASIInvadersFormation* Formation)
{
	MyFormation = Formation;
}

void AASIInvaderActor::Move(const FVector& DeltaLocation)
{
	AddActorLocalOffset(DeltaLocation, true);
}

void AASIInvaderActor::LateralBoundReached(const AActor* BoundActorCollidedWith)
{
	MyFormation->LateralBoundReached(BoundActorCollidedWith);
}

void AASIInvaderActor::HandleDestruction(AActor* DestroyerActor)
{
	// TODO: VFXs
	//UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());

	// TODO: Sounds
	//UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

	// TODO: Fire CallBack for GameMode (Score) and Formation (Update Amount of Invaders and Grid Composition)
	//AASIBaseProjectile* ProjectileActor = Cast<AASIBaseProjectile>(DestroyerActor);
	//if (IsValid(ProjectileActor) && OnPlayerPawnKilled.IsBound())
	//{
	//	OnPlayerPawnKilled.Broadcast();
	//}

	Destroy(false, true);
}
