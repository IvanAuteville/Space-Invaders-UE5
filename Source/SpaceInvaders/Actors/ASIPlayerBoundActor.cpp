// Copyright Ivan Ariel Garcia Auteville.


#include "ASIPlayerBoundActor.h"
#include "Components/SceneComponent.h"

AASIPlayerBoundActor::AASIPlayerBoundActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SceneComp->SetMobility(EComponentMobility::Static);
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetMobility(EComponentMobility::Static);
	StaticMeshComp->SetupAttachment(RootComponent);

	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetNotifyRigidBodyCollision(false);
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// Object Type: WorldStatic
	// Ignore everything, Block Pawn
}

