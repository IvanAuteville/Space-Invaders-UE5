// Copyright Ivan Ariel Garcia Auteville.


#include "ASIBaseProjectile.h"

// Sets default values
AASIBaseProjectile::AASIBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASIBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASIBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

