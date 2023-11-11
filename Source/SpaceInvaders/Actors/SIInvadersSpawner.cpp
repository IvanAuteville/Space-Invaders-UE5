// Copyright Ivan Ariel Garcia Auteville.


#include "SIInvadersSpawner.h"
#include "Components/SceneComponent.h"

ASIInvadersSpawner::ASIInvadersSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// Init Components
	SpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPosition"));
	SetRootComponent(SpawnPosition);
}

//void ASIInvadersSpawner::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//void ASIInvadersSpawner::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

