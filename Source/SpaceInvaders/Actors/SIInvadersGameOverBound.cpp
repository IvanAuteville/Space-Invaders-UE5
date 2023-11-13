// Copyright Ivan Ariel Garcia Auteville.


#include "SIInvadersGameOverBound.h"
#include "Components/StaticMeshComponent.h"
#include "SpaceInvaders/Enemies/ASIInvaderActor.h"

ASIInvadersGameOverBound::ASIInvadersGameOverBound()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetMobility(EComponentMobility::Static);
	SetRootComponent(StaticMeshComp);

	StaticMeshComp->SetGenerateOverlapEvents(true);
	StaticMeshComp->SetNotifyRigidBodyCollision(false);
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// Object Type: WorldStatic
	// Ignore everything, Overlap WorldDynamic
}

void ASIInvadersGameOverBound::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void ASIInvadersGameOverBound::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StaticMeshComp->OnComponentBeginOverlap.Clear();
	OnInvadersReachedBottom.Clear();
}

void ASIInvadersGameOverBound::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this)
		return;

	AASIInvaderActor* InvaderActor = Cast<AASIInvaderActor>(OtherActor);
	if (!IsValid(InvaderActor))
		return;

	if(!OnInvadersReachedBottom.IsBound())
		return;
	
	OnInvadersReachedBottom.Broadcast();
}
