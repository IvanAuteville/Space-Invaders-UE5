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
	StaticMeshComp->SetGenerateOverlapEvents(false);
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

void AASIInvaderActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr || OtherActor == this)
		return;

	// NOTE: it would be a better practice to handle this through a Destructible Interface, 
	// but I'm running low on time and this is a very simple way of handling it
	AASIPlayerPawn* PlayerPawn = Cast<AASIPlayerPawn>(OtherActor);
	if (!IsValid(PlayerPawn))
		return;
	
	PlayerPawn->HandleDestruction(this);
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
