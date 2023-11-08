// Copyright Ivan Ariel Garcia Auteville.


#include "ASIInvadersBounds.h"
#include "Components/BoxComponent.h"

AASIInvadersBounds::AASIInvadersBounds()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxComp);
}

//void AASIInvadersBounds::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//void AASIInvadersBounds::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

