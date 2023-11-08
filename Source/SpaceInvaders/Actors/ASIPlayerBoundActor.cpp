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

	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// OBJ TYPE: WorldStatic
	// Ignore everything, Block Pawn
}

//void AASIPlayerBoundActor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

//void AASIPlayerBoundActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

