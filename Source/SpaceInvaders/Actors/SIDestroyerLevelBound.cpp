// Copyright Ivan Ariel Garcia Auteville.


#include "SIDestroyerLevelBound.h"
#include "Components/StaticMeshComponent.h"
#include "SpaceInvaders/Enemies/ASIUFOActor.h"

ASIDestroyerLevelBound::ASIDestroyerLevelBound()
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

void ASIDestroyerLevelBound::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

void ASIDestroyerLevelBound::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr || OtherActor == this)
		return;

	//UE_LOG(LogTemp, Warning, TEXT("Destroyer Level Bound collisioned with %s"), *OtherActor->GetName());

	// TODO: Interface Implemented, but short on time to change it!
	// NOTE: it would be a better practice to handle this through a Destructible Interface, 
	// but I'm running low on time and this is a very specific case for the UFO
	// (when reaching level lateral bounds)
	AASIUFOActor* UFOActor = Cast<AASIUFOActor>(OtherActor);
	if (IsValid(UFOActor))
	{
		UFOActor->HandleDestruction(this);
	}
	else
	{
		OtherActor->Destroy();
	}
}

