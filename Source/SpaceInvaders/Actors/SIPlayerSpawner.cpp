// Copyright Ivan Ariel Garcia Auteville.


#include "SIPlayerSpawner.h"
#include "Components/SceneComponent.h"

ASIPlayerSpawner::ASIPlayerSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// Init Components
	SpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPosition"));
	SetRootComponent(SpawnPosition);
}

//void ASIPlayerSpawner::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//void ASIPlayerSpawner::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

