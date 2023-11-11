// Copyright Ivan Ariel Garcia Auteville.


#include "ASIInvaderActor.h"

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
	// Ignore everything, Block WorldStatic (barriers, bunkers) and WorldDynamic (projectiles)

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(StaticMeshComp);
}

//void AASIInvaderActor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

//void AASIInvaderActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

