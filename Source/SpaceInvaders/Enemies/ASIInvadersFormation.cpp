// Copyright Ivan Ariel Garcia Auteville.


#include "ASIInvadersFormation.h"
#include "SpaceInvaders/Enemies/SIFormationDataAsset.h"
#include "SpaceInvaders/Enemies/ASIInvaderActor.h"
#include "Engine/TimerHandle.h"

AASIInvadersFormation::AASIInvadersFormation()
{
	PrimaryActorTick.bCanEverTick = true;

	// Init Components
	FormationLeftRightCornerLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(FormationLeftRightCornerLocation);
}

void AASIInvadersFormation::BeginPlay()
{
	Super::BeginPlay();
	
	// TEST: Delay - GameMode Call, etc.
	SpawnInvaders();
}

void AASIInvadersFormation::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AASIInvadersFormation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASIInvadersFormation::SpawnInvaders()
{
	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FRotator SpawnRotation = GetActorRotation();

	int32 FormationRow = 0;

	const FVector2D InvadersSpacing = FormationData->InvadersSpacing;

	for (const FInvaderTypeRowPair& InvaderTypeRowPair : FormationData->InvaderTypeRowsToSpawn)
	{
		for (int32 CurrentRow = 0; CurrentRow < InvaderTypeRowPair.AmountOfRows; ++CurrentRow)
		{
			for (int32 CurrentCol = 0; CurrentCol < FormationData->AmountOfInvadersPerRow; ++CurrentCol)
			{
				const FVector SpawnLocation = GetActorLocation() + FVector(CurrentCol * InvadersSpacing.X, FormationRow * InvadersSpacing.Y, 0.0);

				AASIInvaderActor* NewInvader = World->SpawnActor<AASIInvaderActor>(InvaderTypeRowPair.InvaderType,
					SpawnLocation, SpawnRotation, SpawnParams);
				NewInvader->SetActorHiddenInGame(true);

				Invaders.Add(NewInvader);
			}

			++FormationRow;
		}
	}

	// Test Delay and inverse order, like in the original Space Invaders
	RunRevealSequence();
}

void AASIInvadersFormation::RunRevealSequence()
{
	InvaderSequenceIndex = Invaders.Num();

	GetWorld()->GetTimerManager().SetTimer(
		InvadersSpawnSequenceTimerHandle,	// handle to cancel timer at a later time
		this,								// the owning object
		&ThisClass::InvaderSpawnSequence,	// function to call on elapsed
		MaxSpawnSequenceTime,				// float delay until elapsed
		true,								// looping?
		SpawnSequenceDelayTime);			// first delay
}

// TODO POLISH: right to left order
void AASIInvadersFormation::InvaderSpawnSequence()
{
	--InvaderSequenceIndex;

	if (InvaderSequenceIndex < 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(InvadersSpawnSequenceTimerHandle);
	}
	else
	{
		AASIInvaderActor* Invader = Invaders[InvaderSequenceIndex];
		Invader->SetActorHiddenInGame(false);
	}
}
