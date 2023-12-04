// Copyright Ivan Ariel Garcia Auteville.


#include "ASIInvadersFormation.h"
#include "Engine/TimerHandle.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

#include "SpaceInvaders/Enemies/SIFormationDataAsset.h"
#include "SpaceInvaders/Enemies/ASIInvaderActor.h"
#include "SpaceInvaders/GameModes/ASIGameModeBase.h"
#include "SpaceInvaders/Projectiles/ASIBaseProjectile.h"

//#define UE_LOG_ENABLED
//#define DEBUG_FORMATION_ENABLED

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

	AASIGameModeBase* MyGameMode = Cast<AASIGameModeBase>(UGameplayStatics::GetGameMode(this));
	MyGameMode->OnGamePaused.AddDynamic(this, &ThisClass::OnGamePaused);

	UpdateCurrentRow();
	UpdateDestinationRow();

	SpawnInvaders();
	SpawnProjectiles();
}

void AASIInvadersFormation::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AASIInvadersFormation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#ifdef DEBUG_FORMATION_ENABLED
	for (int32 i = 0; i < FormationGrid.Num(); ++i)
	{
		const FInvaderFormationSlot& InvaderFormationSlot = FormationGrid[i];

		const FString DebugText = FString::FormatAsNumber(InvaderFormationSlot.Row) + FString(",") +
			FString::FormatAsNumber(InvaderFormationSlot.Column) + FString("[") + FString::FormatAsNumber(i) + FString("]");

		DrawDebugString(GetWorld(), InvaderFormationSlot.Invader->GetActorLocation(), DebugText,
			nullptr, FColor::White, 0.0f, false, 1.0f);
	}
#endif // DEBUG_FORMATION_ENABLED

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

	OriginalInvadersCount = Invaders.Num();

	// Test Delay and "Spawn" individually, like in the original Space Invaders
	RunRevealSequence();
}

// TODO: for the Reverse PowerUp we will need some more checks
void AASIInvadersFormation::LateralBoundReached(const AActor* BoundActorCollidedWith)
{
	if(LastBoundActorCollidedWith == BoundActorCollidedWith)
		return;

	LastBoundActorCollidedWith = BoundActorCollidedWith;

	HorizontalMovementType = (HorizontalMovementType == EHorizontalMovementType::Right) ? 
	EHorizontalMovementType::Left : EHorizontalMovementType::Right;

#ifdef UE_LOG_ENABLED
	UE_LOG(LogTemp, Warning, TEXT("LateralBoundReached!"));
#endif // UE_LOG_ENABLED

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
		RunFireTimer();
	}
	else
	{
		AASIInvaderActor* Invader = Invaders[InvaderSequenceIndex];
		Invader->SetActorHiddenInGame(false);
	}
}

void AASIInvadersFormation::OnGamePaused(const bool bPaused)
{
	SetActorTickEnabled(!bPaused);
	InvadersFormationState = bPaused ? EInvadersFormationState::Stopped : EInvadersFormationState::Moving;
}

bool AASIInvadersFormation::ShouldMove() const
{
	return (InvadersFormationState == EInvadersFormationState::Moving || 
		InvadersFormationState == EInvadersFormationState::Slowed);
}

// TODO POLISH: move one by one with a bit of delay, like the original
void AASIInvadersFormation::Move(const float DeltaTime)
{
	const FThreatLevelValues ThreatLevelValues = ThreatLevelSettings.FindRef(FormationThreatLevel);
	double SpeedModifiers = (InvadersFormationState == EInvadersFormationState::Slowed) ? SlowMultiplier : 1.0;
	double CalculatedMovementSpeed = BaseMovementSpeed * SpeedModifiers * ThreatLevelValues.InvadersSpeedMultiplier * DeltaTime;

	const double HorizontalDir = (HorizontalMovementType == EHorizontalMovementType::Right) ? 1.0 : -1.0;
	const double VerticalDir = (VerticalMovementType == EVerticalMovementType::Up) ? -1.0 : 1.0;

	const double HorizontalSpeedMult = (InvadersFormationAxisMovement == EInvadersFormationAxisMovement::Horizontal) ? 1.0 : 0.0;
	const double VerticalSpeedMult = (InvadersFormationAxisMovement == EInvadersFormationAxisMovement::Vertical) ? 1.0 : 0.0;

	const double HorizontalSpeed = HorizontalDir * HorizontalSpeedMult;
	const double VerticalSpeed = VerticalDir * VerticalSpeedMult;

	const FVector LocalOffset(HorizontalSpeed * CalculatedMovementSpeed, VerticalSpeed * CalculatedMovementSpeed, 0.0);
	
	for (auto It = Invaders.CreateIterator(); It; ++It)
	{
		// An Invader was killed during iteration!
		if (!It)
			continue;

		AASIInvaderActor* Invader = *It;
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

#ifdef UE_LOG_ENABLED
	UE_LOG(LogTemp, Warning, TEXT("-----------------------------"));
	UE_LOG(LogTemp, Warning, TEXT("CHECK: CurrentActorY [%.2f]"), CurrentActorY);
	UE_LOG(LogTemp, Warning, TEXT("CHECK: DestinationRowY [%.2f]"), CurrentRowY);
	UE_LOG(LogTemp, Warning, TEXT("CHECK: DestinationRowY [%.2f]"), DestinationRowY);
	UE_LOG(LogTemp, Warning, TEXT("-----------------------------"));
#endif // UE_LOG_ENABLED

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

#ifdef UE_LOG_ENABLED
	UE_LOG(LogTemp, Warning, TEXT("UPDATE: CurrentRowY [%.2f]"), CurrentRowY);
#endif // UE_LOG_ENABLED
}

void AASIInvadersFormation::UpdateDestinationRow()
{
	const double RowDir = (InvadersFormationState == EInvadersFormationState::Reverse) ? 1.0 : -1.0;
	double RowSpacing = FormationData->InvadersSpacing.Y * RowDir;
	DestinationRowY = CurrentRowY + FormationData->InvadersSpacing.Y;

#ifdef UE_LOG_ENABLED
	UE_LOG(LogTemp, Warning, TEXT("UPDATE: DestinationRowY [%.2f]"), DestinationRowY);
#endif // UE_LOG_ENABLED
}

void AASIInvadersFormation::InvaderDestroyed(AASIInvaderActor* Invader)
{
	Invaders.Remove(Invader);

	UpdateFormationGridOnInvaderDestroyed(Invader);
	UpdateFormationThreatLevel();

	if (OnInvaderKilled.IsBound())
	{
		OnInvaderKilled.Broadcast(Invader);
	}

	if (Invaders.IsEmpty() && OnInvadersDefeated.IsBound())
	{
		OnInvadersDefeated.Broadcast();
	}
}

void AASIInvadersFormation::UpdateFormationGridOnInvaderDestroyed(AASIInvaderActor* Invader)
{
	FInvaderFormationSlot* FoundSlot = FindInvaderFormationSlot(Invader);

	if (FoundSlot)
	{
		FoundSlot->Invader = nullptr;
	}
	else
	{
		checkNoEntry();
	}
}

void AASIInvadersFormation::UpdateFormationThreatLevel()
{
	const int32 CurrentInvadersCount = Invaders.Num();
	const double CurrentInvadersPercentage = static_cast<double>(CurrentInvadersCount) /
		static_cast<double>(OriginalInvadersCount);

	double LowestPercentageChecked = std::numeric_limits<double>::max();

	for (const auto& ThreatEntry : ThreatLevelSettings)
	{
		const EFormationThreatLevel ThreatLevel = ThreatEntry.Key;
		const double PercentageToActivate = ThreatEntry.Value.InvadersPercentageToActivate;

		const bool bShouldUpdate = CurrentInvadersPercentage <= PercentageToActivate &&
			LowestPercentageChecked >= PercentageToActivate;

		if (!bShouldUpdate)
			continue;

		FormationThreatLevel = ThreatLevel;
		LowestPercentageChecked = PercentageToActivate;
	}
}

void AASIInvadersFormation::SpawnProjectiles()
{
	for (int32 i = 0; i < NumberOfProjectiles; ++i)
	{
		AASIBaseProjectile* Projectile = GetWorld()->SpawnActor<AASIBaseProjectile>(
			ProjectileClass,
			GetActorLocation(),
			GetActorRotation());

		Projectile->SetOwner(this);
		Projectile->OnProjectileReady.AddDynamic(this, &ThisClass::OnProjectileReady);

		ProjectilesReady.Add(Projectile);
	}
}

void AASIInvadersFormation::OnProjectileReady(AASIBaseProjectile* Projectile)
{
	ProjectilesInUse.Remove(Projectile);
	ProjectilesReady.Add(Projectile);
}

float AASIInvadersFormation::CalculateRandomFireTime() const
{
	// TODO: can be improved / be exposed -> Enemies should fire more frequently based on FormationThreatLevel
	const float CalculatedMaxShootTime = FMath::Clamp(MaxShootTime * (1.0 / static_cast<double>(FormationThreatLevel)),
		MinShootTime, MaxShootTime);

	return FMath::FRandRange(MinShootTime, CalculatedMaxShootTime);
}

void AASIInvadersFormation::RunFireTimer()
{
	const float FireDelay = CalculateRandomFireTime();
	GetWorld()->GetTimerManager().SetTimer(
		FireTimerHandle,					// handle to cancel timer at a later time
		this,								// the owning object
		&ThisClass::TryToFire,				// function to call on elapsed
		FireDelay,							// float delay until elapsed
		false);								// looping?
}

void AASIInvadersFormation::TryToFire()
{
	if (!ProjectilesReady.IsEmpty() && bShoot)
	{
		AASIInvaderActor* InvaderAttacker = FindInvaderToFire();
		if (IsValid(InvaderAttacker))
		{
			FInvaderFormationSlot* FoundSlot = FindInvaderFormationSlot(InvaderAttacker);

#ifdef UE_LOG_ENABLED
			UE_LOG(LogTemp, Warning, TEXT("Invader: [R = %d, C = %d] will Fire!"), FoundSlot->Row, FoundSlot->Column);
#endif // UE_LOG_ENABLED

			AASIBaseProjectile* Projectile = ProjectilesReady.Last();
			ProjectilesReady.RemoveAt(ProjectilesReady.Num() - 1);
			ProjectilesInUse.Add(Projectile);

			const USceneComponent* ProjectileSpawnPoint = InvaderAttacker->GetProjectileSpawnPoint();

			Projectile->SetActorLocationAndRotation(ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation(), false, nullptr, ETeleportType::TeleportPhysics);

			Projectile->Fire();
		}
	}

	if (!Invaders.IsEmpty())
	{
		RunFireTimer();
	}
}

AASIInvaderActor* AASIInvadersFormation::FindInvaderToFire()
{
	if (Invaders.IsEmpty())
		return nullptr;

	AASIInvaderActor* Shooter = nullptr;
	while (!IsValid(Shooter) && !Invaders.IsEmpty())
	{
		const int32 RandomInvaderIndex = FMath::RandRange(0, Invaders.Num() - 1);
		AASIInvaderActor* RandomInvader = Invaders[RandomInvaderIndex];
		
		FInvaderFormationSlot* FoundSlot = FindInvaderFormationSlot(RandomInvader);

#ifdef UE_LOG_ENABLED
		UE_LOG(LogTemp, Warning, TEXT("RandomInvaderToCheck: [R = %d, C = %d]"), FoundSlot->Row, FoundSlot->Column);
#endif // UE_LOG_ENABLED

		if (!InvaderHasFreePathToFire(*FoundSlot))
			continue;

		Shooter = RandomInvader;
	}

	return Shooter;
}

bool AASIInvadersFormation::InvaderHasFreePathToFire(const FInvaderFormationSlot& FormationSlot)
{
	const int32 InvaderRow = FormationSlot.Row;
	const int32 InvaderCol = FormationSlot.Column;
	const int32 NextUnitIndex = CalcUnitIndex(InvaderRow + 1, InvaderCol);

#ifdef UE_LOG_ENABLED
	UE_LOG(LogTemp, Warning, TEXT("RandomInvaderToCheck -> Next Unit Index: [%d]"), NextUnitIndex);
#endif // UE_LOG_ENABLED

	// We are on the Last Row
	if (NextUnitIndex >= FormationGrid.Num())
	{
		return true;
	}

	// Below me there's empty space!
	const FInvaderFormationSlot& NewFormationSlot = FormationGrid[NextUnitIndex];

#ifdef UE_LOG_ENABLED
	UE_LOG(LogTemp, Warning, TEXT("InvaderBelowToCheck: [R = %d, C = %d]"), NewFormationSlot.Row, NewFormationSlot.Column);
#endif // UE_LOG_ENABLED

	if (!IsValid(NewFormationSlot.Invader))
	{
		return true;
	}

	return false;
}

int32 AASIInvadersFormation::CalcUnitIndex(const int32 Row, const int32 Col) const
{
	return (Row * FormationData->AmountOfInvadersPerRow) + Col;
}

FInvaderFormationSlot* AASIInvadersFormation::FindInvaderFormationSlot(AASIInvaderActor* Invader)
{
	static auto FindInvaderFormationSlotByInvaderActor = [](const FInvaderFormationSlot& FormationSlot, const AASIInvaderActor* Invader)
	{
		return FormationSlot.Invader == Invader;
	};

	FInvaderFormationSlot* FoundSlot = FormationGrid.FindByPredicate([&](const FInvaderFormationSlot& Element)
	{
		return FindInvaderFormationSlotByInvaderActor(Element, Invader);
	});

	return FoundSlot;
}

void AASIInvadersFormation::OnPlayerPawnDestroyed()
{
	for (auto& Projectile : ProjectilesInUse)
	{
		if (ProjectilesReady.Contains(Projectile))
			continue;
		
		ProjectilesReady.Add(Projectile);
	}

	ProjectilesInUse.Reset();
}
