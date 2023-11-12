// Copyright Ivan Ariel Garcia Auteville.


#include "SIInvadersLateralBound.h"
#include "Components/StaticMeshComponent.h"
#include "SpaceInvaders/Enemies/ASIInvaderActor.h"

ASIInvadersLateralBound::ASIInvadersLateralBound()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetMobility(EComponentMobility::Static);
	SetRootComponent(StaticMeshComp);

	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetNotifyRigidBodyCollision(false);
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// Object Type: WorldStatic
	// Ignore everything, Block WorldDynamic
}

void ASIInvadersLateralBound::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

void ASIInvadersLateralBound::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr || OtherActor == this)
		return;

	AASIInvaderActor* Invader = Cast<AASIInvaderActor>(OtherActor);
	if(!IsValid(Invader))
		return;

	Invader->LateralBoundReached(this);
}
