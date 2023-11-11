// Copyright Ivan Ariel Garcia Auteville.


#include "ASIBunkerActor.h"

AASIBunkerActor::AASIBunkerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Init Components
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComp);
	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetNotifyRigidBodyCollision(false);
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
}

//void AASIBunkerActor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

//void AASIBunkerActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

