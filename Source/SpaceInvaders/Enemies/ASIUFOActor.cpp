// Copyright Ivan Ariel Garcia Auteville.


#include "ASIUFOActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AASIUFOActor::AASIUFOActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SceneComp->SetMobility(EComponentMobility::Movable);
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetMobility(EComponentMobility::Movable);
	StaticMeshComp->SetupAttachment(RootComponent);

	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetNotifyRigidBodyCollision(false);
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// Object Type: WorldDynamic
	// Ignore everything, Block WoldDynamic (Projectiles)

}

//void AASIUFOActor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

//void AASIUFOActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

