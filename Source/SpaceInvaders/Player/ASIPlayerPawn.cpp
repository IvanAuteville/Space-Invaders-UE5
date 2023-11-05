// Copyright Ivan Ariel Garcia Auteville.


#include "ASIPlayerPawn.h"

AASIPlayerPawn::AASIPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AASIPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AASIPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASIPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

