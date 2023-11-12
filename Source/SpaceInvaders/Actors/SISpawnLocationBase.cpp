// Copyright Ivan Ariel Garcia Auteville.


#include "SISpawnLocationBase.h"
#include "Components/SceneComponent.h"

ASISpawnLocationBase::ASISpawnLocationBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Init Components
	SpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPosition"));
	SetRootComponent(SpawnPosition);
}

