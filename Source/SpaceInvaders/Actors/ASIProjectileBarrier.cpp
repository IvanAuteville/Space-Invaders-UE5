// Copyright Ivan Ariel Garcia Auteville.


#include "ASIProjectileBarrier.h"
#include "Components/SceneComponent.h"

AASIProjectileBarrier::AASIProjectileBarrier()
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
	StaticMeshComp->SetGenerateOverlapEvents(true);
	StaticMeshComp->SetNotifyRigidBodyCollision(true);
}

//void AASIProjectileBarrier::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

//void AASIProjectileBarrier::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

