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

	UpdateCurrentRow();
	UpdateDestinationRow();

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

	if (!ShouldMove())
		return;
	
	Move(DeltaTime);
}

void AASIInvadersFormation::SpawnInvaders()
{
	InvadersFormationState = EInvadersFormationState::Spawning;

	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FRotator SpawnRotation = GetActorRotation();
	const FVector2D InvadersSpacing = FormationData->InvadersSpacing;

	int32 FormationRow = 0;
	for (const FInvaderTypeRowPair& InvaderTypeRowPair : FormationData->InvaderTypeRowsToSpawn)
	{
		for (int32 CurrentRow = 0; CurrentRow < InvaderTypeRowPair.AmountOfRows; ++CurrentRow)
		{
			for (int32 CurrentCol = 0; CurrentCol < FormationData->AmountOfInvadersPerRow; ++CurrentCol)
			{
				const FVector SpawnLocation = GetActorLocation() + 
				FVector(CurrentCol * InvadersSpacing.X, FormationRow * InvadersSpacing.Y, 0.0);

				AASIInvaderActor* NewInvader = World->SpawnActor<AASIInvaderActor>(InvaderTypeRowPair.InvaderType,
					SpawnLocation, SpawnRotation, SpawnParams);
				NewInvader->SetFormation(this);
				NewInvader->SetActorHiddenInGame(true);

				Invaders.Add(NewInvader);
				FormationGrid.Add(FInvaderFormationSlot(CurrentCol, FormationRow, NewInvader));
			}

			++FormationRow;
		}
	}

	// Test Delay and "Spawn" individually, like in the original Space Invaders
	RunRevealSequence();
}

// TODO: for the Reverse Powerup we will need some more checks
void AASIInvadersFormation::LateralBoundReached(const AActor* BoundActorCollidedWith)
{
	if(LastBoundActorCollidedWith == BoundActorCollidedWith)
		return;

	LastBoundActorCollidedWith = BoundActorCollidedWith;

	HorizontalMovementType = (HorizontalMovementType == EHorizontalMovementType::Right) ? 
	EHorizontalMovementType::Left : EHorizontalMovementType::Right;

	UE_LOG(LogTemp, Warning, TEXT("LateralBoundReached!"));

	InvadersFormationAxisMovement = EInvadersFormationAxisMovement::Vertical;
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
		InvadersFormationState = EInvadersFormationState::Moving;
	}
	else
	{
		AASIInvaderActor* Invader = Invaders[InvaderSequenceIndex];
		Invader->SetActorHiddenInGame(false);
	}
}

bool AASIInvadersFormation::ShouldMove() const
{
	return (InvadersFormationState == EInvadersFormationState::Moving || 
		InvadersFormationState == EInvadersFormationState::Slowed);
}

// TODO POLISH: move one by one with a bit of delay, like the original
void AASIInvadersFormation::Move(const float DeltaTime)
{
	// TODO: take into account EFormationThreatLevel::FormationThreatLevel
	double SpeedModifiers = (InvadersFormationState == EInvadersFormationState::Slowed) ? SlowMultiplier : 1.0;
	double CalculatedMovementSpeed = BaseMovementSpeed * SpeedModifiers * DeltaTime;

	const double HorizontalDir = (HorizontalMovementType == EHorizontalMovementType::Right) ? 1.0 : -1.0;
	const double VerticalDir = (VerticalMovementType == EVerticalMovementType::Up) ? -1.0 : 1.0;

	const double HorizontalSpeedMult = (InvadersFormationAxisMovement == EInvadersFormationAxisMovement::Horizontal) ? 1.0 : 0.0;
	const double VerticalSpeedMult = (InvadersFormationAxisMovement == EInvadersFormationAxisMovement::Vertical) ? 1.0 : 0.0;

	const double HorizontalSpeed = HorizontalDir * HorizontalSpeedMult;
	const double VerticalSpeed = VerticalDir * VerticalSpeedMult;

	const FVector LocalOffset(HorizontalSpeed * CalculatedMovementSpeed, VerticalSpeed * CalculatedMovementSpeed, 0.0);
	
	for (AASIInvaderActor* Invader : Invaders)
	{	
		Invader->Move(LocalOffset);
	}

	// Move Formation Actor (Rows Only)
	const FVector FormationLocalOffset(0.0, VerticalSpeed * CalculatedMovementSpeed, 0.0);
	AddActorLocalOffset(LocalOffset, false);

	if (InvadersFormationAxisMovement == EInvadersFormationAxisMovement::Vertical && IsDestinationRowReached())
	{
		UpdateCurrentRow();
		UpdateDestinationRow();

		InvadersFormationAxisMovement = EInvadersFormationAxisMovement::Horizontal;
	}
}

bool AASIInvadersFormation::IsDestinationRowReached() const
{
	const double CurrentActorY = GetActorLocation().Y;

	UE_LOG(LogTemp, Warning, TEXT("-----------------------------"));
	UE_LOG(LogTemp, Warning, TEXT("CHECK: CurrentActorY [%.2f]"), CurrentActorY);
	UE_LOG(LogTemp, Warning, TEXT("CHECK: DestinationRowY [%.2f]"), CurrentRowY);
	UE_LOG(LogTemp, Warning, TEXT("CHECK: DestinationRowY [%.2f]"), DestinationRowY);
	UE_LOG(LogTemp, Warning, TEXT("-----------------------------"));

	if (FMath::IsNearlyEqual(CurrentActorY, DestinationRowY, UE_KINDA_SMALL_NUMBER))
		return true;

	if (InvadersFormationState == EInvadersFormationState::Reverse)
	{
		return CurrentActorY < DestinationRowY;
	}

	return CurrentActorY > DestinationRowY;
}

void AASIInvadersFormation::UpdateCurrentRow()
{
	CurrentRowY = GetActorLocation().Y;

	UE_LOG(LogTemp, Warning, TEXT("UPDATE: CurrentRowY [%.2f]"), CurrentRowY);
}

void AASIInvadersFormation::UpdateDestinationRow()
{
	const double RowDir = (InvadersFormationState == EInvadersFormationState::Reverse) ? 1.0 : -1.0;
	double RowSpacing = FormationData->InvadersSpacing.Y * RowDir;
	DestinationRowY = CurrentRowY + FormationData->InvadersSpacing.Y;

	UE_LOG(LogTemp, Warning, TEXT("UPDATE: DestinationRowY [%.2f]"), DestinationRowY);
}